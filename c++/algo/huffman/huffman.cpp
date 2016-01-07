#include "huffman.hpp"

Huffman::Huffman()
{
    letter[28]= {'a','b','c','d','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' ','\0'};
}

int main(){

    Huffman* huff = new Huffman();
    std::string test = "tagada";
    std::cout<<test<<std::endl;
    std::cout<<test.size()<<std::endl;
    bool done = false;

    char* p = huff->letter;

    while(done == false){
        for(unsigned int i=0; i<test.size(); ++i)
            for(int i=0; i<28; ++i){

            }
    }



    delete huff;

    return 0;
}
