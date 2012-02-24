/// \file ODAHandle.h
/// @author Piotr Kuchta - ECMWF Aug 2011

#ifndef ODAHandle_H
#define ODAHandle_H

#include <machine.h>

#include "Buffer.h"
#include "Date.h"
#include "Time.h"
#include "Offset.h"
#include "ODATranslator.h"


class ODAHandle {
public:
	ODAHandle(Offset, Offset);
	~ODAHandle(); // Change to virtual if base class

	void addValue(const string& columnName, double v);

	template <typename T>
		void getValue(const string& name, T& value)
	{
		value = ODATranslator<T>()(values_[name]);
		Log::debug() << "ODAHandle::getValue('" << name << "',=>" << value << ")" << endl;
	}

	void print(ostream&) const;

	Offset start() { return start_; }
	void start(const Offset& n) { start_ = n; }

	Offset end() { return end_; }
	void end(const Offset& n) { end_ = n; }

private:
// No copy allowed
	ODAHandle(const ODAHandle&);
	ODAHandle& operator=(const ODAHandle&);

	Offset start_;
	Offset end_;
	map<string, double> values_;

	friend ostream& operator<<(ostream& s, const ODAHandle& p)
		{ p.print(s); return s; }

};

#endif
