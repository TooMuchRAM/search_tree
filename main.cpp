#include <iostream>
#include <vector>
#include <fstream>

using namespace std;


struct Property {
    const string name;
    const vector<string> values;
};
struct Split;

struct SplitChildren {
    vector<Split> splits;
    string attribute;
};

struct Split {
    Property property;
    vector<SplitChildren> children;
    double gain;
};


vector<string> headers;
vector<vector<string>> content;


void readTable(const string &filename);

int size(const vector<string> &vec);

int size(const vector<vector<string>> &vec);

int size(const vector<Property> &vec);

void split(const vector<vector<string>> &rows, vector<Split> &splits);

int main() {
    readTable("../illness.txt");
    vector<Split> splits = {};
    split(content, splits);
    return 0;
}

int valueInVector(const string &value, const vector<string> &vec) {
    int x = 0;
    for (int i = 0; i < size(vec); i++) {
        if (vec[i] == value) {
            x++;
        }
    }
    return x;
}

bool isValueInVector(const string &value, const vector<string> &vec) {
    int x = valueInVector(value, vec);
    return x != 0;
}

Property lastProperty(const vector<Property> &properties) {
    return properties[size(properties) - 1];
}

vector<Property> readProperties(const vector<vector<string>> &content2) {

    vector<Property> properties;

    for (int i = 0; i < size(headers); i++) {
        vector<string> values;
        for (int j = 0; j < size(content2); j++) {
            string value = content2[j][i];
            if (!isValueInVector(value, values)) {
                values.push_back(value);
            }
        }
        Property property = {
                .name = headers[i],
                .values = values
        };
        properties.push_back(property);
    }

    return properties;
}

vector<vector<string>>
filterOnAttribute(const vector<vector<string>> &rows, const int column, const string &attribute) {
    vector<vector<string>> result;

    for (int i = 0; i < size(rows); i++) {
        if (rows[i][column] == attribute) {
            result.push_back(rows[i]);
        }
    }

    return result;
}

float calculateGini(const Property &property, const vector<vector<string>> &rows) {
    vector<string> results = {};
    for (int i = 0; i < size(rows); i++) {
        results.push_back(rows[i][size(rows[i]) - 1]);
    }

    float gini = 1;

    for (int i = 0; i < size(property.values); i++) {
        auto occurences = float(valueInVector(property.values[i], results));
        float x = occurences / float(size(results));
        gini -= x * x;
    }

    return gini;
}

void split(const vector<vector<string>> &rows, vector<Split> &splits) {
    vector<Property> properties = readProperties(rows);
    float wholeGini = calculateGini(lastProperty(properties), rows);

    if (wholeGini == 0) {
        return;
    }

    float bestSplitGain = 0;
    int bestSplitIndex = 0;
    vector<Split> splitbuffer = {};
    for (int i = 0; i < size(properties)-1; i++) {
        float gain = wholeGini;
        int j = 0;
        while (j < size(properties[i].values)) {
            vector<vector<string>> filteredRows = filterOnAttribute(rows, i, properties[i].values[j]);
            vector<Property> filteredProperties = readProperties(filteredRows);
            const float attGini = calculateGini(lastProperty(filteredProperties), filteredRows);
            gain -= float(size(filteredRows)) / float(size(rows)) * attGini;
            j++;
        }

        Split currentSplit = Split{
                .property = properties[i],
                .children = {},
                .gain = gain
        };
        if (gain >= bestSplitGain) {
            bestSplitGain = gain;
            bestSplitIndex = static_cast<int>(splitbuffer.size());
        }
        splitbuffer.push_back(currentSplit);
    }
    Split bestSplit = splitbuffer[bestSplitIndex];
    for (int i=0; i<size(bestSplit.property.values); i++) {
        vector<vector<string>> filteredRows = filterOnAttribute(rows, bestSplitIndex, bestSplit.property.values[i]);

        vector<Split> childSplits = {};
        split(filteredRows, childSplits);
        bestSplit.children.push_back(SplitChildren{
            .splits = childSplits,
            .attribute = bestSplit.property.values[i]
        });
    }
    splits.push_back(bestSplit);
}

void readTable(const string &filename) {
    ifstream input(filename);

    char c;
    string buffer;
    vector<string> vecbuffer = {};
    int i = 0;
    while (input.get(c)) {
        if (c != '\t' && c != '\n') {
            buffer += c;
        }
        if (c == '\t') {
            vecbuffer.push_back(buffer);
            buffer = "";
        }
        if (c == '\n') {
            vecbuffer.push_back(buffer);
            buffer = "";
            if (i > 0) {
                content.push_back(vecbuffer);
            } else {
                headers = vecbuffer;
                buffer = "";
            }
            vecbuffer = {};
            i++;
        }
    }
    if (size(vecbuffer) != 0) {
        vecbuffer.push_back(buffer);
        buffer = "";
        content.push_back(vecbuffer);
        vecbuffer = {};
    }
}

int size(const vector<string> &vec) {
    return static_cast<int>(vec.size());
}

int size(const vector<vector<string>> &vec) {
    return static_cast<int>(vec.size());
}

int size(const vector<Property> &vec) {
    return static_cast<int>(vec.size());
}
