#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <iomanip>
#include <chrono>
using namespace std::chrono;
typedef steady_clock Clock;

// TODO

// map functions

using namespace std;

unsigned int hashFunction(char const* key, int table_size) {
    //Begin
    unsigned int hashCode;
    unsigned int b;

    //   Initiate hash code for a character array(your key) with 0.
    hashCode = 0;
    //        Traverse the character array from left to right(from 0 index to the end of the array) :
    for (unsigned int i = 0; i < strlen(key); i++) {

        //        If n is even :
        if (i % 2 == 0) {
            //               b = (Hash code << 7) ^ an ^ (Hash code >> 3)
            b = (hashCode << 7) ^ int(key[i]) ^ (hashCode >> 3);
        }
        //        If n is odd :
        else {
            //               b = (Hash code << 11) ^ an ^ (Hash code >> 5)
            b = (hashCode << 11) ^ int(key[i]) ^ (hashCode >> 5);
            //               b = ~b
            b = ~b;
        }
        //        Update hash code : hash code = hash code ^ b
        hashCode = hashCode ^ b;
    }
    //        Traverse end

    //        Set the leftmost bit to 0. (i.e., set the 32nd bit to 0)
    unsigned int mask = 0x7FFFFFFF;
    hashCode = hashCode & mask;
    //        Return hash code
    int position = hashCode % table_size;
    return position;
    //        End
}

class UnorderedMap
{
public:
    class Bucket {
    public:
        struct Student {

            Student* next;
            int index;
            string UFID;
            string name;

            Student(int index, string UFID, string name) {
                this->index = index;
                this->UFID = UFID;
                this->name = name;
                this->next = nullptr;
            }
            Student* getNext() {
                return next;
            }
            void setNext(Student* next) {
                this->next = next;
            }
            string getUFID() {
                return UFID;
            }
            string getName() {
                return name;
            }
            int getIndex() {
                return index;
            }
        };
    private:
        int index;
        int numStudents;
        Student* head;
    public:
        Bucket(int index) {
            this->index = index;
            numStudents = 0;
            head = nullptr;
        }
        void addStudent(string UFID, string NAME) {

            if (head == nullptr) {
                head = new Student(index, UFID, NAME);
            }
            else {
                Student* tempHead = head;
                head = new Student(index, UFID, NAME);
                head->setNext(tempHead);
            }
        }
        Student* getHead() {
            return head;
        }
        void addOne() {
            numStudents += 1;
        }
        void takeOne() {
            numStudents -= 1;
        }
        int getIndex() {
            return index;
        }
        int getNumStudents() {
            return numStudents;
        }
        void setHead(Student* stud) {
            Student* temp = head;
            head = stud;
            delete temp;
        }

    };

private:
    //define your data structure here
    vector<Bucket> buckets;
    //define other attributes e.g. bucket count, maximum load factor, size of table, etc. 
    unsigned int bucketCount;
    double maxLoadFactor;
    int sizeTable;

public:
    class Iterator;
    UnorderedMap(unsigned int bucketCount, double loadFactor);
    ~UnorderedMap();
    Iterator begin() const;
    Iterator end() const;
    std::string& operator[] (std::string const& key);
    void rehash();
    void remove(std::string const& key);
    unsigned int size();
    double loadFactor();


    class Iterator
    {
    private:


        int numStud;
        string key;
        string value;
        vector<Bucket> bucks;
        int buckNum;
        int studgo;
        int kids;

    public:
        //this constructor does not need to be a default constructor;
        //the parameters for this constructor are up to your discretion.
        //hint: you may need to pass in an UnorderedMap object.
        Iterator(UnorderedMap map) {

            numStud = 0;
            bucks = map.buckets;
            Bucket* buck = &map.buckets.at(0);
            while (buck->getNumStudents() == 0) {
                buck++;
            }
            Bucket::Student* stud = buck->getHead();

            kids = map.size();

            if (stud != nullptr) {
                key = stud->getUFID();
                value = stud->getName();
                buckNum = buck->getIndex();
            }
            else {
                key = "";
                value = "";
                buckNum = 0;
            }

            studgo = 0;
        }

        Iterator& operator=(Iterator const& rhs) {
            key = rhs.key;
            value = rhs.value;
            numStud = rhs.numStud;

            return *this;
        }
        Iterator& operator++() {
            numStud += 1;
            if (numStud == kids) {
                key = "";
                value = "";
                numStud = 0;
                return *this;
            }
            int x = 0;
            Bucket buck = bucks.at(x);
            while (buck.getNumStudents() == 0) {
                x++;
                buck = bucks.at(x);
            }
            Bucket::Student* stud = buck.getHead();
            for (int i = 0; i < numStud; i++) {
                if (stud->getNext() == nullptr) {
                    x += 1;
                    buck = bucks.at(x);
                    stud = buck.getHead();
                    studgo = 0;
                    while (stud == nullptr) {
                        x += 1;
                        buck = bucks.at(x);
                        stud = buck.getHead();
                    }
                }
                else {
                    stud = stud->getNext();
                    studgo += 1;
                }

            }
            buckNum = x;
            key = stud->getUFID();
            value = stud->getName();
            return *this;
        }
        bool operator!=(Iterator const& rhs) {
            bool different = true;
            if (key == rhs.key && value == rhs.value && numStud == rhs.numStud) {
                different = false;
            }
            return different;
        }
        bool operator==(Iterator const& rhs) {
            bool same = true;
            if (key != rhs.key || value != rhs.value || numStud == rhs.numStud) {
                same = false;
            }
            return same;
        }
        std::pair<std::string, std::string> operator*() const {
            pair<string, string> val(key, value);
            return val;
        }
        friend class UnorderedMap;
    };
};

UnorderedMap::UnorderedMap(unsigned int bucketCount, double loadFactor)
{
    this->bucketCount = bucketCount;
    this->maxLoadFactor = loadFactor;
    this->sizeTable = 0;

    for (unsigned int i = 0; i < bucketCount; i++) {
        buckets.push_back(Bucket(i));
    }


}

UnorderedMap::~UnorderedMap()
{

}

UnorderedMap::Iterator UnorderedMap::begin() const
{
    Iterator iter = Iterator(*this);
    return iter;
}

UnorderedMap::Iterator UnorderedMap::end() const
{
    Iterator iter = Iterator(*this);
    iter.numStud = 0;
    iter.key = "";
    iter.value = "";
    return iter;
}

std::string& UnorderedMap::operator[] (std::string const& key)
{
    int x = hashFunction(key.c_str(), bucketCount);
    Bucket::Student* st = this->buckets.at(x).getHead();
    if (st != nullptr) {
        while (st != nullptr) {
            if (st->getUFID() == key) {
                return st->name;
            }
            else st = st->getNext();
        }
    }
    string name = "";
    buckets.at(x).addOne();
    buckets.at(x).addStudent(key, name);
    if (loadFactor() >= maxLoadFactor) {
        rehash();
        x = hashFunction(key.c_str(), bucketCount);
    }

    return buckets.at(x).getHead()->name;

}

void UnorderedMap::rehash()
{
    bucketCount = bucketCount * 2;
    vector<Bucket> newBucks;
    string name;
    string num;
    for (unsigned int i = 0; i < bucketCount; i++) {
        newBucks.push_back(Bucket(i));
    }
    Iterator iter = Iterator(*this);
    while (iter != this->end()) {
        name = iter.value;
        num = iter.key;
        int x = hashFunction(num.c_str(), bucketCount);
        newBucks.at(x).addStudent(num, name);
        newBucks.at(x).addOne();
        ++iter;
    }

    buckets = newBucks;

}

void UnorderedMap::remove(std::string const& key)
{

    int x = hashFunction(key.c_str(), bucketCount);
    Bucket* buck = &buckets.at(x);
    Bucket::Student* sud = buck->getHead();

    if (sud->UFID == key) {
        buckets.at(x).setHead(sud->getNext());
    }
    else {
        while (sud->UFID != key) {
            sud = sud->next;
        }
        Bucket::Student* sudp = buck->getHead();
        while (sudp->next->UFID != key) {
            sudp = sudp->next;
        }
        sudp->setNext(sud->getNext());
    }
    buckets.at(x).takeOne();
}

unsigned int UnorderedMap::size()
{
    int num = 0;
    for (unsigned int i = 0; i < buckets.size(); i++) {
        num += buckets.at(i).getNumStudents();
    }
    return num;
}

double UnorderedMap::loadFactor()
{
    double lF = ((double)this->size()) / (double)bucketCount;

    return lF;
}

//Do not change main() 
int main()
{
    int lines = 0, buckets = 0;
    double maxLoadFactor = 0.0;
    std::string command = "", ufid = "", name = "", key = "";
    std::cin >> lines >> buckets >> maxLoadFactor;
    UnorderedMap myMap = UnorderedMap(buckets, maxLoadFactor);
    while (lines--)
    {
        std::cin >> command;
        if (command == "hash")
        {
            std::cin >> key;
            const char* convertedKey = key.c_str();
            std::cout << hashFunction(convertedKey, buckets) << "\n";
        }
        else if (command == "insert")
        {
            std::cin >> ufid >> name;
            myMap[ufid] = name;
        }
        else if (command == "size")
        {
            std::cout << myMap.size() << "\n";
        }
        else if (command == "load")
        {
            std::cout << std::fixed << std::setprecision(2) << myMap.loadFactor() << "\n";
        }
        else if (command == "search")
        {
            std::cin >> ufid;
            std::cout << myMap[ufid] << "\n";
        }
        else if (command == "traverse")
        {
            for (UnorderedMap::Iterator iter = myMap.begin(); iter != myMap.end(); ++iter)
            {
                std::cout << (*iter).first << " " << (*iter).second << "\n";
            }

            /* This should also work
                for (auto tableEntry: myMap)
                {
                    std::cout << tableEntry.first << " " << tableEntry.second << "\n";
                }
            */
        }
        else if (command == "remove")
        {
            std::cin >> ufid;
            myMap.remove(ufid);
        }
    }

    return 0;
}