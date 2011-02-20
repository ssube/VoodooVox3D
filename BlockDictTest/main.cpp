
#include "BlockDictionary.hpp"
#include "BlockTemplate.hpp"

int main(int argc, char * argv[])
{
    printf("Starting block dictionary test\n");
    BlockDictionary * dict = new BlockDictionary();

    printf("\tAdding attributes (i, f, b)\n");
    //dict->AddAttribute("iID");
    //dict->AddAttribute("bOcclude");
    //dict->AddAttribute("fTexture");
    //dict->AddAttribute("fSpeed");
    //dict->AddAttribute("sName");
    
    printf("\tCreating template\n");
    BlockTemplate * temp1 = new BlockTemplate();
    temp1->ID = 1;
    temp1->Occludes = false;
    temp1->Visible = false;
    temp1->Texture = 0;
    temp1->Speed = 1.0f;
    strcpy(temp1->Name, "Noodles");

    printf("\tCreating template\n");
    BlockTemplate * temp2 = new BlockTemplate();
    temp2->ID = 2;
    temp2->Occludes = false;
    temp2->Visible = false;
    temp2->Texture = 0;
    temp2->Speed = 1.0f;
    strcpy(temp2->Name, "Stuff");

    BlockTemplate * temp3 = new BlockTemplate();
    temp3->ID = 13;
    temp3->Occludes = false;
    temp3->Visible = true;
    temp3->Texture = 1;
    temp3->Speed = 0.2f;
    strcpy(temp3->Name, "Stone");

    BlockTemplate * temp4 = new BlockTemplate();
    temp4->ID = 14;
    temp4->Occludes = true;
    temp4->Visible = true;
    temp4->Texture = 2;
    temp4->Speed = 0.2f;
    strcpy(temp4->Name, "Wood");

    BlockTemplate * temp5 = new BlockTemplate();
    temp5->ID = 15;
    temp5->Occludes = false;
    temp5->Visible = true;
    temp5->Texture = 3;
    temp5->Speed = 0.2f;
    strcpy(temp5->Name, "Fruit");

    printf("\tAdding templates\n");
    dict->AddTemplate(temp1);
    dict->AddTemplate(temp2);
    dict->AddTemplate(temp3);
    dict->AddTemplate(temp4);
    dict->AddTemplate(temp5);

    printf("Serializing dict\n");
    unsigned long size = 0;
    void * dictBuffer = dict->Serialize(&size);
    printf("\tSize: %d\n", size);

    printf("Deserializing buffer\n");
    BlockDictionary * dict2 = BlockDictionary::Deserialize(dictBuffer);

    printf("Test complete.\n");

    dict->ToFile("blocks.dict");
}
