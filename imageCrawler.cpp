#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stack>
#define TAG "<img"

using namespace std;



class Crawler {
    private:
    string getFileName() {
        static int count = 1;
        return (to_string(count++) + ".txt");
    }

    void getPage(string url, string filePath, string fileName) {
        string fileLocation = filePath + "htmlPages/" + fileName;
        string command = "wget -O " + fileLocation + " " + url;
        system(command.c_str());
        this->fetchImageLinks(fileLocation, filePath, url);
    }

    void getImage(string url, string filePath) {
        string command = "wget -P " + filePath + " " + url;
        system(command.c_str());
    }

    string getTag(string s, int i) {
        stack<int> a;
        string img = "";
        do{
            img += s[i];
            if(s[i] == '<')
                a.push(1);
            if(s[i] == '>')
                a.pop();
            i++;
        }while(!a.empty());
        return img;
    }

    string getAttributeValue(string tag, string attribute) {
        int i = tag.find(attribute);
        string value = "";

        if(i > tag.length()) 
            return value;

        i += attribute.length();
        char starter = tag[i];
        while(tag[++i] != starter) 
            value += tag[i];
        
        return value;
    }

    int filter(string http) {
        int len = http.length();
        return (http.find(".png") < len || http.find(".jpeg") < len || http.find(".png") < len);
    }

    string readFile(string location) {
        FILE *ptr = fopen(location.c_str(), "r");

        if(!ptr) { // Check if file exist or not
            cout << "ERROR 404 NOT FILE NOT FOUND" << endl;
            exit(0);
        }

        struct stat st; // if file exist
        stat(location.c_str(),&st);
        int size=st.st_size;
        string fileData = "";
        char ch = getc(ptr);
        while(ch != EOF) {
            fileData += ch;
            ch = getc(ptr);
        }

        return fileData;
    }

    void fetchImageLinks(string location, string parentdir, string url) {
        string code = readFile(location);
        int index = -1, x = 0;
        int count = 0;
        while((index = code.find(TAG, index+1)) < code.length()) {
            count++;
            string tag = getTag(code, index);
            string aValue = this->getAttributeValue(tag, "src=");
            // if(aValue == "") continue;
            // cout << tag << endl << endl;
            if(aValue == "" || !this->filter(aValue) || !this->validURL(aValue)) continue;
            this->getImage(aValue, parentdir+"images");
        }
        cout << count << endl;
    }

    int validURL(string url) {
        string command = "wget --spider " + url;
        return !system(command.c_str());
    }

    public:
    void start(string url, string filePath) {
        this->getPage(url, filePath, this->getFileName());
    }
};


int main(int argc, char const *argv[])
{
    Crawler crawler;
    crawler.start(argv[1], "/home/rishav/Desktop/ImageCrawler/");
    return 0;
}