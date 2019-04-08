HashTable::HashTable(int hashSize){
  hashTable = new wordItem*[hashSize];
  for(int k = 0; k < hashSize; k++){
    hashTable[k] = nullptr;
  }
  hashTableSize = hashSize;
  numItems = 0;
  numCollisions = 0;
}

//look a change

HashTable::~HashTable(){
  wordItem* temp;
  wordItem* next;
  for(int k = 0; k < hashTableSize; k++){
    if(hashTable[k] != nullptr){
      temp = hashTable[k];
      next = hashTable[k]->next;
      while(next != nullptr){
        delete temp;
        temp = next;
        next = next->next;
      }
      delete temp;
    }
  }
}

unsigned int HashTable::getHash(string word){
  unsigned int hashValue = 5381;
  int length = word.length();
  for(int k = 0; k < length; k++){
    hashValue = ((hashValue << 5) + hashValue) + word[k];
  }
  hashValue = hashValue % hashTableSize;
  return hashValue;
}

void HashTable::addWord(string word){
  int hashValue = getHash(word);
  if(hashTable[hashValue] == nullptr){
    hashTable[hashValue] = new wordItem;
    hashTable[hashValue]->count = 1;
    hashTable[hashValue]->word = word;
    hashTable[hashValue]->next = nullptr;
    numItems++;
    return;
  }
  wordItem* temp = hashTable[hashValue];
  while(temp->word != word && temp->next != nullptr){
    temp = temp->next;
  }
  if(temp->word == word){
    temp->count++;
  }else{
    temp->next = new wordItem;
    temp->next->count = 1;
    temp->next->word = word;
    temp->next->next = nullptr;
    numCollisions++;
    numItems++;
  }
  return;
}

/*void HashTable::printTable(){
  wordItem* temp;
  for(int k = 0; k < hashTableSize; k++){
    cout << "Slot " << k+1 << ":" << endl;
    if(hashTable[k] != nullptr){
      temp = hashTable[k];
      while(temp != nullptr){
        cout << "Word: " << temp->word << " Count: " << temp->count << endl;
        temp = temp->next;
      }
    }
  }
}*/

wordItem* HashTable::searchTable(string word){
  wordItem* temp = hashTable[getHash(word)];
  while(temp != nullptr){
    if(temp->word == word) return temp;
    temp = temp->next;
  }
  return nullptr;
}

bool HashTable::isInTable(string word){
  if(searchTable(word) == nullptr) return false;
  else return true;
}

void HashTable::incrementCount(string word){
  wordItem* temp = searchTable(word);
  temp->count++;
}

int HashTable::getNumCollisions(){
  return numCollisions;
}

int HashTable::getNumItems(){
  return numItems;
}

int HashTable::getTotalNumWords(){
  wordItem* temp;
  int total = 0;
  for(int k = 0; k < hashTableSize; k++){
    if(hashTable[k] != nullptr){
      temp = hashTable[k];
      while(temp != nullptr){
        total += temp->count;
        temp = temp->next;
      }
    }
  }
  return total;
}

void HashTable::printTopN(int number){
  // cout << getHash("was") << endl;
  // wordItem* wasPTR = searchTable("was");
  // if(wasPTR) cout << wasPTR->word << " " << wasPTR->count << endl;
  wordItem* table[number];
  int intTable[number];
  string wordTable[number];
  wordItem* temp;
  int minValue = 0, maxValue = 0, counter;
  bool flag = false, fullFlag = true, bigFlag = false;
  for(int k = 0; k < number; k++){
    table[k] = nullptr;
  }
  // cout << "ok to here?" << endl;
  for(int k = 0; k < hashTableSize; k++){
    // cout << k << endl;
    temp = hashTable[k];
    if(temp != nullptr){
      for(int j = 0; j < number; j++){
        if(table[j] == nullptr) fullFlag = false;
      }
      while(temp != nullptr && fullFlag == false){
        for(int j = 0; j < number; j++){
          if(table[j] == nullptr && flag == false){
            table[j] = temp;
            flag = true;
            // cout << j << " " << table[j]->word << " " << table[j]->count << endl;
          }
        }
        flag = false;
        temp = temp->next;
      }
      for(int j = 0; j < number; j++){
        if(table[j] == nullptr) fullFlag = false;
      }
      if(fullFlag == true){
        while(bigFlag == false){
          bigFlag = true;
          // cout << "it gets in here now?" << endl;
          for(int j = 0; j < number; j++){
            // cout << "does this print?" << endl;
            if(table[j]->count < table[minValue]->count){
              minValue = j;
              bigFlag = false;
              // cout << j << " " << table[minValue]->word << " " << table[minValue]->count << endl;
            }
          }
          // cout << table[minValue]->word << " " << table[minValue]->count << endl;
          while(temp != nullptr && flag == false){
            // cout << "does it get in here?" << endl;
            if(temp->count > table[minValue]->count){
              table[minValue] = temp;
              flag = true;
              // cout << "success" << endl;
            }
            // cout << table[minValue]->word << " " << table[minValue]->count << endl;
            // cout << table[0]->word << " " << table[0]->count << endl;
            temp = temp->next;
          }
          flag = false;
        }
        minValue = 0;
      }
    }
    bigFlag = false;
    fullFlag = true;
    flag = false;
  }
  int totalNumWords = getTotalNumWords();
  wordItem notptrTable[number];
  for(int k = 0; k < number; k++){
    notptrTable[k] = *(table[k]);
  }
  // cout << "makes it to here?" << endl;
  for(int k = 0; k < number; k++){
    intTable[k] = -1;
  }
  for(int k = 0; k < number; k++){
    for(int j = 0; j < number; j++){
      if(notptrTable[j].count > maxValue){
        if(flag == false){
          maxValue = notptrTable[j].count;
          counter = j;
        }
      }
      flag = false;
    }
    intTable[k] = notptrTable[counter].count;
    wordTable[k] = notptrTable[counter].word;
    notptrTable[counter].count = 0;
    maxValue = 0;
  }
  for(int k = 0; k < number; k++){
    cout << fixed << setprecision(4) << (float)(intTable[k])/(float)(totalNumWords) << " - " << wordTable[k] << endl;
  }
  return;
}

void getStopWords(char *ignoreWordFileName, HashTable &stopWordsTable){
  ifstream ignorestream;
  string temp;
  ignorestream.open(ignoreWordFileName);
  while(getline(ignorestream, temp)){
    stopWordsTable.addWord(temp);
  }
  ignorestream.close();
}

bool isStopWord(string word, HashTable &stopWordsTable){
  for(int k = 0; k < 50; k++){
    if(stopWordsTable.isInTable(word))
      return true;
  }
  return false;
}
