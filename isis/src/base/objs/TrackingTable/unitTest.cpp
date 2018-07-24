#include <iostream>

#include "IException.h"
#include "Preference.h"
#include "Table.h"
#include "TableField.h"
#include "TableRecord.h"
#include "TrackingTable.h"

using namespace Isis;
using namespace std;

int main(int argc, char *argv[]) {

  Preference::Preferences(true);
  
  try{
    cout << "Unit test for TrackingTable\n" << endl;
    
    cout << "Testing constructor with a Table object ..." << endl;
    
    cout << "Constructing Table ..." << endl;
    
    TableField fileNameField("FileName", TableField::Text, 50);
    TableField serialNumberField("SerialNumber", TableField::Text, 50);

    
    TableRecord record;
    record += fileNameField;
    record += serialNumberField;
    Table tableIn("TestingTable", record);
    
    record[0] = "fileName1.cub";
    record[1] = "1234567890";
    tableIn += record;
    
    record[0] = "fileName2.cub";
    record[1] = "123";
    tableIn += record;
    
    record[0] = "fileName3.dat";
    record[1] = "456789";
    tableIn += record;
    
    cout << "First record : " << QString(tableIn[0][0]) << ", " << QString(tableIn[0][1]) << endl;
    cout << "Second record: " << QString(tableIn[1][0]) << ", " << QString(tableIn[1][1]) << endl;
    cout << "Third record : " << QString(tableIn[2][0]) << ", " << QString(tableIn[2][1]) << endl;
    
    TrackingTable trackingTable(tableIn);
    
    cout << "TrackingTable object created" << endl;
    
    cout << endl;
    
    cout << "Testing the indexToFileName method ..." << endl;
    
    for (int i = 0; i < 4; i++) {
      try {
        cout << "FileName at index " << i << ": " << trackingTable.indexToFileName(i).name() << endl;
      }
      catch (IException e) {
        cout << "FileName at index " << i << " does not exist and an exception is thrown." << endl;
        e.print();
      }
    }

    cout << endl;

    cout << "Testing the fileNameToIndex method ..." << endl;
    
    cout << "Index of FileName fileName1.cub: " 
           << trackingTable.fileNameToIndex("fileName1.cub", "1234567890") << endl;
    cout << "Index of FileName fileName2.cub: " 
           << trackingTable.fileNameToIndex("fileName2.cub", "123") << endl;
    cout << "Index of FileName fileName3.cub: " 
           << trackingTable.fileNameToIndex("fileName3.dat", "456789") << endl;
    cout << "Index of the non-existent FileName fileName4.cub (demonstrating its addition): " 
           << trackingTable.fileNameToIndex("fileName4.cub", "12345678901234567890") << endl;
           
    cout << endl;

    cout << "Testing the toTable method ..." << endl;
    
    Table tableOut = trackingTable.toTable();
    
    cout << "First record : " << QString(tableOut[0][0]) << ", " << QString(tableOut[0][1]) << endl;
    cout << "Second record: " << QString(tableOut[1][0]) << ", " << QString(tableOut[1][1]) << endl;
    cout << "Third record : " << QString(tableOut[2][0]) << ", " << QString(tableOut[2][1]) << endl;
    cout << "Fourth record: " << QString(tableOut[3][0]) << ", " << QString(tableOut[3][1]) << endl;
    
    cout << endl;
    
    cout << "Creating a new TrackingTable object with the table returned from toTable method ..." << endl;
    
    TrackingTable trackingTable2(tableOut);
    
    cout << "New TrackingTable object created" << endl;
    
    cout << endl;
    
    cout << "Testing that the Table returned from toTable on new TrackingTable matches ..." << endl;
             
    Table tableOut2 = trackingTable2.toTable();
             
    cout << "First record : " << QString(tableOut2[0][0]) << ", " << QString(tableOut2[0][1]) << endl;
    cout << "Second record: " << QString(tableOut2[1][0]) << ", " << QString(tableOut2[1][1]) << endl;
    cout << "Third record : " << QString(tableOut2[2][0]) << ", " << QString(tableOut2[2][1]) << endl;
    cout << "Fourth record: " << QString(tableOut2[3][0]) << ", " << QString(tableOut2[3][1]) << endl;
    
  }
  catch (IException &e) {
    cout << "Unit test failed." << endl;
    e.print();
  }
  
  
}
