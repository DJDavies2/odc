// File SQLDatabase.h
// Baudouin Raoult - ECMWF Dec 03

#ifndef SQLDatabase_H
#define SQLDatabase_H

#include "PathName.h"
#include "SQLTable.h"

class DataHandle;

namespace odb {
namespace sql {
// Forward declarations

class SQLStatement;
namespace expression { class SQLExpression; }
using namespace expression;

typedef map<string, set<string> > Links;
typedef map<string, SQLExpression*> Variables;

class SQLDatabase {
public:
	typedef enum { ODA, CSV } DataFormat;

	SQLDatabase(const PathName&,const string&);
	SQLDatabase(const string& = "default");
	virtual ~SQLDatabase(); // Change to virtual if base class

// -- Methods
	virtual void open();
	virtual void close();

	virtual SQLTable* table(const string&);
	virtual SQLTable* openDataHandle(DataHandle&, DataFormat = ODA) = 0; 
	virtual SQLTable* openDataStream(istream&, DataFormat = CSV) = 0; 
	virtual void addTable(SQLTable *table) { tablesByName_[table->name()] = table; };

	void setLinks(const Links&);
	void setLinks() { setLinks(links_); }

	void addLinks(const Links& ls) { links_.insert(ls.begin(), ls.end()); }
	Links& links() { return links_; }

	virtual const string& name() const { return name_; }

	SQLExpression* getVariable(const string&) const;
	void   setVariable(const string&, SQLExpression*);

	virtual bool sameAs(const SQLDatabase& other) const;
	SchemaAnalyzer& schemaAnalyzer() { return schemaAnalyzer_; }

protected:
	Links links_;
	map<string,SQLTable*> tablesByName_;
	PathName path_;

	Variables variables_;
	string name_;
	SchemaAnalyzer schemaAnalyzer_;

private:
// No copy allowed
	SQLDatabase(const SQLDatabase&);
	SQLDatabase& operator=(const SQLDatabase&);

	void loadIOMAP();
	void loadDD();
	void loadFLAGS();

// -- Friends
	//friend ostream& operator<<(ostream& s,const SQLDatabase& p)
	//	{ p.print(s); return s; }
};

} // namespace sql 
} // namespace odb 

#endif
