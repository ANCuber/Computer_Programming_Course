#include <cstring>
#include <cstdlib>
#include <iostream>
using namespace std;

class PatternElement {
private:
    char* validChars_;         // Characters in the character class
    size_t validCharsCount_;   // Number of valid characters
    size_t minOccurrences_;    // Minimum number of occurrences
    size_t maxOccurrences_;    // Maximum number of occurrences

    friend class PatternMatcher;

public:
    PatternElement();
    ~PatternElement();
    PatternElement(const PatternElement& other);
    PatternElement& operator=(const PatternElement& other);
    size_t getMinOccurrences() const { return minOccurrences_; }
    size_t getMaxOccurrences() const { return maxOccurrences_; }
    const char* getValidChars() const { return validChars_; }
    size_t getValidCharsCount() const { return validCharsCount_; }
};

class PatternMatcher {
private:
    char* pattern_;                // Original pattern string
    PatternElement* elements_;     // Array of pattern elements
    size_t elementsCount_;         // Number of pattern elements
    size_t patternLength_;         // Length of pattern string

public:
    PatternMatcher(const char* pattern);
    ~PatternMatcher();
    PatternMatcher(const PatternMatcher&) = delete;
    PatternMatcher& operator=(const PatternMatcher&) = delete;
    bool matches(const char* input);
};
//PREPEND END

//TEMPLATE BEGIN
// TODO: Implement PatternElement methods
PatternElement::PatternElement() {
    // Initialize member variables
    // Allocate memory for validChars_
    validCharsCount_ = 0;
    minOccurrences_ = 0;
    maxOccurrences_ = 0;
    validChars_ = new char[300]();
}

PatternElement::~PatternElement() {
    // Clean up allocated memory
    delete[] validChars_;
}

PatternElement::PatternElement(const PatternElement& other) {
    // Implement deep copy
    validCharsCount_ = other.getValidCharsCount();
    minOccurrences_ = other.getMinOccurrences();
    maxOccurrences_ = other.getMaxOccurrences();
    validChars_ = new char[validCharsCount_];
    for(int i = 0; i < validCharsCount_; i++){
        validChars_[i] = other.validChars_[i];
    }
}

PatternElement& PatternElement::operator=(const PatternElement& other) {
    // Implement deep copy assignment
    // Handle self-assignment
    if(this == &other){return *this;}
    delete[] validChars_;
    validCharsCount_ = other.getValidCharsCount();
    minOccurrences_ = other.getMinOccurrences();
    maxOccurrences_ = other.getMaxOccurrences();
    validChars_ = new char[validCharsCount_];
    for(int i = 0; i < validCharsCount_; i++){
        validChars_[i] = other.validChars_[i];
    }
    return *this;
}

// TODO: Implement PatternMatcher methods
PatternMatcher::PatternMatcher(const char* pattern) {
    // Copy pattern
    // Initialize members
    // Parse pattern into elements
    int i = 0;
    patternLength_ = 0;
    elementsCount_ = 0;
    pattern_ = new char [300];
    while(pattern[i] != '\0'){
        pattern_[i] = pattern[i];
        i++;
        patternLength_ ++;
    }
    pattern_[i] = '\0';
    i = 0;
    int j = 0;
    elements_ = new PatternElement [300];
    while (pattern_[i] != '\0'){
        if(pattern_[i] == '[') i++;
        int k = 0;
        while(pattern[i] != ']'){
            (elements_[j]).validChars_[k] = pattern[i];
            (elements_[j]).validCharsCount_ ++;
            k++;
            i++;
            patternLength_ ++;
        }
        elements_[j].validChars_[k] = '\0';
        k++;
        i++;
        if(pattern_[i] == '+'){
            (elements_[j]).maxOccurrences_ = SIZE_MAX;
            (elements_[j]).minOccurrences_ = 1;
            i++;
        }
        else if(pattern_[i] == '*'){
            (elements_[j]).minOccurrences_ = 0;
            (elements_[j]).maxOccurrences_ = SIZE_MAX;
            i++;
        }
        else if(pattern_[i] == '{'){
            i++;
            char* min = new char[100];
            int l = 0;
            while(pattern_[i] != ','){
                min[l] = pattern_[i];
                i++;
                l++;
            }
            elements_[j].minOccurrences_ = atoi(min);
            char* max = new char[100];
            l = 0;
            while(pattern_[i] != '}'){
                max[l] = pattern_[i];
                i++;
                l++;
            }
            i++;
            elements_[j].maxOccurrences_ = atoi(max);
            delete[] min;
            delete[] max;
        }
        /*int l = 0;
        while(elements_[j].validChars_[l] != '\0'){
            cout << elements_[j].validChars_[l] << " ";
            l++;
        }*/
        //cout << endl;
        j++;
        elementsCount_ ++;
    }

}

PatternMatcher::~PatternMatcher() {
    // Clean up allocated memory
    delete[] pattern_;
    delete[] elements_;
}


bool PatternMatcher::matches(const char* input) {
    // extreme cases
    if (!input) {
        return false;
    }

    if (elementsCount_ == 0) {
        return (input[0] == '\0');
    }

    auto matchRecursive = [this](const char* input, size_t ii, size_t ei, size_t count, auto& self) -> bool {
        cout<<"pass"<<endl;
        if (ei == elementsCount_) {
            // base case
            return input[ii] == '\0';
        }

        const PatternElement& currentElement = elements_[ei];
        size_t min = currentElement.getMinOccurrences();
        size_t max = currentElement.getMaxOccurrences();
        cout<<min<<' '<<max<<endl;

        size_t localCount = count; // current match count
        while (localCount <= max) {
            /*if (localCount >= min) {
                if (self(input, ii+1, ei+1, 0, self)) {
                    return true; // next element
                }
                cout << "false returned" <<endl;
            }*/
            // base
            if (input[ii] == '\0') {
                break;
            }
            char currentChar = input[ii];
            bool isValid = false;

            for (size_t i = 0; i < currentElement.getValidCharsCount(); ++i) {
                if (currentChar == currentElement.getValidChars()[i]) {
                    isValid = true;
                    cout << "found" << endl;
                    break;
                }
            }

            if (!isValid) {
                cout << "element index: " << ei << " input index: " << ii << ", break here" << endl;
                break; 
            }

            localCount++;
            cout << "Matched, count: " << localCount << " input index: " << ii << " element index: " << ei << endl;
            ii++;
            
            if (self(input, ii, ei + 1, 0, self)) {
                return true; // next element
            }
        }

        /* 
        if (count >= minOccurrences) {
            cout << "Next pattern: " << ei+1 <<endl;
            return self(input, ii, ei + 1, 0, self);
        }*/
        if(count < min){
            return false; // matched<min
        }else if(count > max){
            return false;
        }else{
            return true;
        }
    };

    return matchRecursive(input, 0, 0, 0, matchRecursive);
}


int main() {
    int T;
    cin >> T;
    
    while(T--) {
        char pattern[1001], str[1001];
        cin >> pattern >> str;
        
        PatternMatcher matcher(pattern);
        cout << (matcher.matches(str) ? "true" : "false") << endl;
    }
    
    return 0;
}