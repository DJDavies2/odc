/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

///
/// \file WriterBufferingIterator.cc
///
/// @author Piotr Kuchta, Feb 2009

#include "eckit/exception/Exceptions.h"

#include "odc/core/Header.h"
#include "odc/WriterBufferingIterator.h"
#include "odc/Writer.h"

using namespace eckit;
using namespace odc::api;

namespace odc {

//----------------------------------------------------------------------------------------------------------------------

WriterBufferingIterator::WriterBufferingIterator(Owner &owner, DataHandle *dh, bool openDataHandle, const odc::sql::TableDef* tableDef)
: refCount_(0),
  owner_(owner),
  columns_(0),
  lastValues_(0),
  nextRow_(0),
  columnOffsets_(0),
  columnByteSizes_(0),
  nrows_(0),
  f(dh),
  encodedDataBuffer_(0),
  path_(owner.path()),
  initialisedColumns_(false),
  properties_(),
  nextRowInBuffer_(0),
  columnsBuffer_(0),
  rowsBufferSize_(owner.rowsBufferSize()),
  tableDef_(tableDef),
  openDataHandle_(openDataHandle)
{
	if (openDataHandle)	
		open();
}

WriterBufferingIterator::~WriterBufferingIterator()
{
    close();
    delete [] lastValues_;
    delete [] nextRow_;
    delete [] columnOffsets_;
    delete [] columnByteSizes_;
    if (! openDataHandle_)
        delete f;
}

unsigned long WriterBufferingIterator::gatherStats(const double* values, unsigned long count)
{
	ASSERT(count == columns().size());

	//for (size_t i = 0; i < columns_.size(); ++i) Log::info() << "gatherStats: columns_[" << i << "]=" << *columns_[i] << std::endl;

    for(size_t i = 0; i < count; i++) {
        columns_[i]->coder().gatherStats(values[columnOffsets_[i]]);
    }

	return 0;
} 

template <typename DATASTREAM>
int WriterBufferingIterator::setOptimalCodecs()
{
	if (columnsBuffer_.size() == 0)
	{
		columnsBuffer_ = columns();
		columnsBuffer_.resetStats();
	}

	return codecOptimizer_.setOptimalCodecs<DATASTREAM>(const_cast<MetaData&>(columns()));
}

void WriterBufferingIterator::allocBuffers()
{
    delete [] lastValues_;
	delete [] nextRow_;
    delete [] columnOffsets_;
    delete [] columnByteSizes_;

    // Don't do anything until evenything is initialised (this check is before we do rowDataSizeDoubles
    // which makes use of the coders).

    for (const Column* column : columns_) ASSERT(column->hasInitialisedCoder());

    // Initialise this value
    rowDataSizeDoubles_ = rowDataSizeDoublesInternal();

    // Allocate arrays

    int32_t numDoubles = rowDataSizeDoubles();
	int32_t colSize = columns().size();

    lastValues_ = new double [numDoubles];
    nextRow_ = new double [numDoubles];
    columnOffsets_ = new size_t[colSize];
    columnByteSizes_ = new size_t[colSize];
    ASSERT(lastValues_);

    // Initialise data

    size_t offset = 0;
    for (int i = 0; i < colSize; ++i) {

        // If we are trying to do anything before the writer is properly initialised ...
        ASSERT(columns_[i]->hasInitialisedCoder());

        nextRow_[i] = lastValues_[i] = columns_[i]->missingValue();
        columnOffsets_[i] = offset;
        columnByteSizes_[i] = columns_[i]->dataSizeDoubles() * sizeof(double);
        offset += columns_[i]->dataSizeDoubles();
    }

	nrows_ = 0;

	encodedDataBuffer_.size(sizeof(uint16_t) + colSize * sizeof(double));
}

void WriterBufferingIterator::allocRowsBuffer()
{
    rowDataSizeDoubles_ = rowDataSizeDoublesInternal();
    rowByteSize_ = sizeof(uint16_t) + rowDataSizeDoubles() * sizeof(double);
    rowsBuffer_ = Buffer(rowsBufferSize_ * rowByteSize_);
	nextRowInBuffer_ = rowsBuffer_;
}

void WriterBufferingIterator::writeHeader()
{
	allocBuffers();
    for (size_t i = 0; i < columns_.size(); ++i) {

        // If we haven't configured a row, then this is bad
        ASSERT(columns_[i]->hasInitialisedCoder());

		columns_[i]->coder().resetStats();
    }
    initialisedColumns_ = true;
	//for (size_t i = 0; i < columns_.size(); ++i) Log::info() << "writeHeader: columns_[" << i << "]=" << *columns_[i] << std::endl;
}

bool WriterBufferingIterator::next()
{
    return writeRow(nextRow_, columns().size()) == 0;
}

double* WriterBufferingIterator::data() { return nextRow_; }
double& WriterBufferingIterator::data(size_t i)
{
    ASSERT(initialisedColumns_);
	ASSERT(i >= 0 && i < columns().size());
    return nextRow_[columnOffsets_[i]];
}

int WriterBufferingIterator::writeRow(const double* data, unsigned long nCols)
{
	ASSERT(nCols == columns().size());
    ASSERT(initialisedColumns_);

	if (rowsBuffer_ == 0)
		allocRowsBuffer();

	gatherStats(data, nCols);

    std::copy(data, data + rowDataSizeDoubles(), reinterpret_cast<double*>(nextRowInBuffer_ + sizeof(uint16_t)));
    nextRowInBuffer_ += rowByteSize_;

	ASSERT(nextRowInBuffer_ <= rowsBuffer_ + rowsBuffer_.size());

    if (nextRowInBuffer_ == rowsBuffer_ + rowsBuffer_.size())
		flush();

    return 0;
}

size_t WriterBufferingIterator::rowDataSizeDoublesInternal() const {

    size_t total = 0;
    for (const auto& column : columns()) {
        total += column->dataSizeDoubles();
    }
    return total;
}

int WriterBufferingIterator::doWriteRow(core::DataStream<core::SameByteOrder>& stream, const double* values)
{
    if (lastValues_ == 0) allocBuffers();

    // Find where the first changing row is

    // BUG: First row may not be properly encoded if it is zero.
    uint16_t k = 0;
    for (; k < columns().size(); ++k) {
        if (::memcmp(&values[columnOffsets_[k]], &lastValues_[columnOffsets_[k]], columnByteSizes_[k]) != 0) break;
    }

    // Marker stores the starting column

    uint8_t marker[2] { (k % 256), ((k / 256) % 256) };
    stream.writeBytes(marker, sizeof(marker)); // raw write

    // TODO: Update Codecs to encode to a DataStream directly.
    // n.b. We are relying on the sizing of the buffer behind stream to have been done correctly.
    //      This is fundamentally unsafe. TODO: Do it properly.

    char* p = stream.get();

    for (size_t i = k; i < columns().size(); ++i)  {
        p = columns_[i]->coder().encode(p, values[columnOffsets_[i]]);
        ::memcpy(&lastValues_[columnOffsets_[i]], &values[columnOffsets_[i]], columnByteSizes_[i]);
	}

    stream.set(p);

	nrows_++;
	return 0;
} 

int WriterBufferingIterator::open()
{
	//Log::debug() << "WriterBufferingIterator::open@" << this << ": Opening data handle " << f << std::endl;
	ASSERT(f);

    Length estimatedLen = 20 * 1024 * 1024;
	f->openForWrite(estimatedLen);

	return 0;
}


int WriterBufferingIterator::setColumn(size_t index, std::string name, api::ColumnType type)
{
	//Log::debug() << "WriterBufferingIterator::setColumn: " << std::endl;
	ASSERT(index < columns().size());
	Column* col = columns_[index];
	ASSERT(col);

    // Ensure that this column is unique!
    for (size_t i = 0; i < columns_.size(); i++) {
        if (index != i && columns_[i] != 0) {
            if (columns_[i]->name() == name) {
                std::stringstream ss;
                ss << "Attempting to create multiple columns with the same name: " << name;
                throw SeriousBug(ss.str(), Here());
            }
        }
    }

	col->name(name); 
	col->type<DataStream<SameByteOrder, FastInMemoryDataHandle> >(type, false);
	return 0;
}

int WriterBufferingIterator::setBitfieldColumn(size_t index, std::string name, api::ColumnType type, eckit::sql::BitfieldDef b)
{
	//Log::debug() << "WriterBufferingIterator::setBitfieldColumn: " << std::endl;
	ASSERT(index < columns().size());
	Column* col = columns_[index];
	ASSERT(col);

	col->name(name); 
	col->type<DataStream<SameByteOrder, FastInMemoryDataHandle> >(type, false);
    col->bitfieldDef(b);
	col->missingValue(0);
	return 0;
}

void WriterBufferingIterator::missingValue(size_t i, double missingValue)
{
	ASSERT(i < columns().size());
	Column* col = columns_[i];
	ASSERT(col);

	col->missingValue(missingValue);
}

void WriterBufferingIterator::flush()
{
	if (nextRowInBuffer_ == rowsBuffer_)
		return;
    
	setOptimalCodecs<DataStream<SameByteOrder, FastInMemoryDataHandle> >();

    Buffer encodedBuffer(rowsBuffer_.size());
    core::DataStream<core::SameByteOrder> encodedStream(encodedBuffer);

    // Iterate over stored rows, and re-encode them into the encodedBuffer

    // TODO: Note we can ensure alignment when storing these. Currently the uint16_t ensure non-alignment.

    size_t rowsWritten = 0;
    unsigned char* p = rowsBuffer_.data();
    while (p < nextRowInBuffer_) {
        doWriteRow(encodedStream, reinterpret_cast<double *>(pr + sizeof(uint16_t)));
        p += rowByteSize_;
        ++rowsWritten;
    }

    // Clean up storage buffers for row data
    allocBuffers();

    Buffer headerBuffer = serializeHeader(memoryDataHandle_.position(), rowsNumber);

    Log::debug() << "WriterBufferingIterator::flush: header size: " << headerBuffer.size() << std::endl;

    dataHandle().write(headerBuffer, headerBuffer.size()); // Write header
    dataHandle().write(encodedBuffer, encodedStream.position()); // Write encoded data

	Log::debug() << "WriterBufferingIterator::flush: flushed " << rowsWritten << " rows." << std::endl;

    // Reset the write buffers

    nextRowInBuffer_ = rowsBuffer_.data();

    // This is a bad place to be. We need to reset the coders in the columns, not clone
    // the existing ones (which have been optimised).

    columns_.resetCodecs();
    columns_.resetStats();
}


void WriterBufferingIterator::serializeHeader(size_t dataSize, size_t rowsNumber) {
    return core::Header::serializeHeader<core::SameByteOrder>(dataSize, rowsNumber, properties_, columns());
}

int WriterBufferingIterator::close()
{
	flush();

	if (!openDataHandle_ && f)
	{
		f->close();
		f = 0;
	}
	return 0;
}

std::vector<eckit::PathName> WriterBufferingIterator::outputFiles()
{
    std::vector<eckit::PathName> r;
    r.push_back(path_);
    return r;
}

//----------------------------------------------------------------------------------------------------------------------

} // namespace odc 

