#include <iostream>

#include <libxml/HTMLparser.h>

#include <libxml/parser.h>

#include <libxml/xpath.h>



using namespace std;



void silentErrorFunc(void * ctx, const char * msg, ...) { //ignore parsing error

    

}



xmlDocPtr getdoc(const char* docname) 

{

    xmlDocPtr doc;

    doc = htmlReadFile(docname, NULL, 0); //parse html file,docname could be a url

    /*if (doc == NULL) {

        printf("Error: unable to parse file.html\n");

        return(-1);

    }*/

    return doc;

}



xmlXPathObjectPtr getnodeset(xmlDocPtr doc, xmlChar* xpath) //xpath is keyword, doc is parsed html 

{

    xmlXPathContextPtr context;

    xmlXPathObjectPtr result;

    context = xmlXPathNewContext(doc);

    //error checking:

    if (context == NULL) {

        printf("Error in xmlXPathNewContext\n");

        return NULL;

    }



    result = xmlXPathEvalExpression(xpath, context); //result is xpathobject, which has nodeset as one of its members

    //error checking:

    if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {

        xmlXPathFreeObject(result);

        printf("No result\n");

        return NULL;

    }

    xmlXPathFreeContext(context);

    return result;



}



int main()

{

    xmlSetGenericErrorFunc(NULL, silentErrorFunc); //ignore parsing error

    xmlDocPtr doc;

    xmlChar* xpath = (xmlChar*) "//*[@class='dtText']"; //class used for definition

    xmlXPathObjectPtr result;

    xmlNodeSetPtr nodes;

    char* text; //definition



    const char* docpath = "/tmp.html";//path to tmp.html, convert to char*

    doc = getdoc(docpath); 

    result = getnodeset(doc, xpath);

    if (result)

    {

        nodes = result->nodesetval;

        for (int i = 0; i < 5 && i < nodes->nodeNr; i++) { //print text for each nodes, max 5

            text = (char*)xmlNodeGetContent(nodes->nodeTab[i]);

            printf("%s\n", text);

            xmlFree(text);



        }

        //cleanup:

        xmlXPathFreeObject(result);

        xmlFreeDoc(doc);



        return 0;

    }

    }
