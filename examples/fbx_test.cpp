#include <iostream>
#include <fstream>

#include "tools/ModelLoader/ReadCommon.h"
#include "tools/ModelLoader/fbx/FbxParser.h"

using namespace std;

typedef struct {
    int type;
    int mode_len;
    int index;
    int count;
} FbxHead;

typedef struct {
    int attribute_type;
    long id;

    int char_count;
    char *name;

    float *position;
    float *rotation;
    float *scale;    
} NodeHead;

int main() {
    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

    filebuf *pbuf;  
    ifstream filestr;  
    long size;  
    char * buffer;

    // 要读入整个文件，必须采用二进制打开   
    filestr.open ("/home/kou/Videos/box.data", ios::binary);
    // 获取filestr对应buffer对象的指针   
    pbuf=filestr.rdbuf();  

    // 调用buffer对象方法获取文件大小  
    size=pbuf->pubseekoff (0,ios::end,ios::in);  
    pbuf->pubseekpos (0,ios::in);  

    // 分配内存空间  
    buffer= new char[size];

    // 获取文件内容  
    pbuf->sgetn (buffer,size); 

    FbxHead *head = (FbxHead*)buffer;

    cout << "type: " << (int)(head->type) << " len: " << head->mode_len << " index: " << head->index << " count: " << head->count << endl;

//    for (int i = 0; i < head->count; ++i) {
//        
//    }
   
    NodeHead *node = (NodeHead*)&buffer[16];

    cout << "attribute type: " << node->attribute_type << " id: " << node->id << "char count: " << node->char_count << endl;


    ////////////////// stream reader /////////////////////////////////
    filestr.seekg(0);

    FbxParser *parser = new FbxParser();

    parser->parse(&filestr);

    cout << "root node count: " << parser->getNodes().size() << endl;

//    int type = reader->ReadInt(&filestr);
//    int mode_len = reader->ReadInt(&filestr);
//    int index = reader->ReadInt(&filestr);
//    int count = reader->ReadInt(&filestr);

//    cout << "type: " << type << " len: " << mode_len << " index: " << index << " count: " << count << endl;
    
    //for (int i = 0; i < count ; ++i) {
    //    
    //}
    
//    int att_type = reader.ReadInt(&filestr);

//    long id = ReadLong(&filestr);

//    cout << " att_type: " << att_type << " id: " << id << endl;

//    string name = ReadString(&filestr);

//    cout << " att_type: " << att_type << " id: " << id << " name: " + name << endl;

    return 0;
}
