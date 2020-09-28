// Длина последовательности
#define BITSTAFF_LENGTH 5

// Массив с насальным кодом
// Вариант 19
int MyCode[] = {1, 0, 1, 0, 1, 0, 0, 1,  // 0xA9
                0, 1, 1, 1, 1, 1, 0, 0,  // 0x7C
                1, 0, 0, 1, 0, 1, 0, 1,  // 0x95
                1, 1, 1, 1, 1, 0, 0, 0,  // 0xF8
                1, 1, 0, 0, 0, 1, 1, 0   // 0xC6
               };

// Массив, в котором будет хранится зашифрованный код
int* BitstaffCode;

// Массив, в котором будет хранится розшифрованный код
int* BitstaffDecode;

// Переменные для хранения размера массивов
int MyCodeSize;
int BitstaffCodeSize;
int BitstaffDecodeSize;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 13; i > 8; i--) {
    pinMode(i, OUTPUT);
  }

  BitstaffCodeSize = 0;
  BitstaffDecodeSize = 0;
  MyCodeSize = sizeof(MyCode) / sizeof(int);

  // Шифрование массива
  BitstaffCode = BitstaffCodingFunc(MyCode, MyCodeSize, BitstaffCode);
  // Определение размера зашифрованного массива
  while (BitstaffCode[BitstaffCodeSize] == 0 || BitstaffCode[BitstaffCodeSize] == 1) {
    BitstaffCodeSize++;
  }

  // Дешифрование массива
  BitstaffDecode = BitstaffDecodingFunc(BitstaffCode, BitstaffCodeSize, BitstaffDecode);
  // Определение размера розшифрованного массива
  while (BitstaffDecode[BitstaffDecodeSize] == 0 || BitstaffDecode[BitstaffDecodeSize] == 1) {
    BitstaffDecodeSize++;
  }



  // Вывод изначального кода
  Serial.println("MyCode :");
  PrintCode8Bit(MyCode, MyCodeSize);
  // Вывод зашифрованого кода
  Serial.println("BitstaffCode :");
  PrintCode8Bit(BitstaffCode, BitstaffCodeSize);
  // Вывод розшифрованого кода
  Serial.println("BitstaffDecode :");
  PrintCode8Bit(BitstaffDecode, BitstaffDecodeSize);
}

void loop() {
  ReturnToZero(MyCode, MyCodeSize);
  ReturnToZero(BitstaffCode, BitstaffCodeSize);
  ReturnToZero(BitstaffDecode, BitstaffDecodeSize);
}

void ReturnToZero(int* Code, int CodeSize){
  for(int i = 0; i < CodeSize; i++){
    digitalWrite(12, HIGH);
    if(Code[i] == 0){
      digitalWrite(11, HIGH);
    } else if (Code[i] == 1){
      digitalWrite(9, HIGH);
    }
    delay(500);
    digitalWrite(9, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, HIGH);
    delay(500);
    digitalWrite(10, LOW);
    digitalWrite(12, LOW);
    delay(500);
  }
  digitalWrite(13, HIGH);
  delay(2000);
  digitalWrite(13, LOW);
}

void PrintCode8Bit(int *yourArray, int Size) {
  int temp = 1;
  Serial.println();
  for (int i = 0; i < Size; i++) {
    Serial.print(yourArray[i]);
    Serial.print(" ");
    if (temp % 8 == 0) {
      Serial.println();
    }
    temp++;
  }
  Serial.println();
}

int BitstaffCodingFunc(int MyCode[], int MyCodeSize, int* BitstaffCode) {

  // Временная переменная, для подсчета длины последовательности единиц
  int OneCount = 0;

  // Переменная для определения количества вставленных нулей
  int BitstaffCount = 0;

  // Инициализация массива с позициями для вставки нулей
  int* BitstaffPos;
  BitstaffPos = new int[MyCodeSize];
  for (int i = 0; i < MyCodeSize; i++) {
    BitstaffPos[i] = -1;
  }

  // Определение позиций для вставки нулей
  for (int i = 0; i < MyCodeSize; i++) {
    if (MyCode[i] == 1) {
      OneCount++;
    } else if (MyCode[i] == 0) {
      OneCount = 0;
    }
    if (OneCount == BITSTAFF_LENGTH) {
      BitstaffPos[BitstaffCount] = i + 1 + BitstaffCount;
      BitstaffCount++;
    }
  }

  // Инициализация нового массива, для хранения зашифрованого кода
  int BitstaffCodeSize = (8 - BitstaffCount % 8) + BitstaffCount + MyCodeSize;
  BitstaffCode = new int[BitstaffCodeSize];
  for (int i = 0; i < BitstaffCodeSize; i++) {
    BitstaffCode[i] = 0;
  }

  // Шифрование кода
  int i_BitstaffPos = 0;
  int i_MyCode = 0;
  for (int i = 0; i < BitstaffCodeSize; i++) {
    if (i == BitstaffPos[i_BitstaffPos]) {
      BitstaffCode[i] = 0;
      i_BitstaffPos++;
      i++;
    }
    if (i_MyCode < MyCodeSize) {
      BitstaffCode[i] = MyCode[i_MyCode];
      i_MyCode++;
    }
  }
  int* BitstaffTemp = new int[BitstaffCount];
  for (int i = 0; i < BitstaffCount; i++) {
    BitstaffTemp[i] = BitstaffCode[MyCodeSize + i];
    BitstaffCode[MyCodeSize + i] = 0;
  }
  int j = 0;
  for (int i = BitstaffCount; i > 0; i--) {
    BitstaffCode[BitstaffCodeSize - i] = BitstaffTemp[j];
    j++;
  }
  // Конец шифрования

  // Возврат результата
  return BitstaffCode;
}

int BitstaffDecodingFunc(int BitstaffCode[], int BitstaffCodeSize, int* BitstaffDecode) {

  // Временная переменная, для подсчета длины последовательности единиц
  int OneCount = 0;

  // Переменная для определения количества вставленных нулей
  int BitstaffCount = 0;

  // Инициализация массива с позициями вставленных нулей
  int* BitstaffPos;
  BitstaffPos = new int[BitstaffCodeSize];
  for (int i = 0; i < BitstaffCodeSize; i++) {
    BitstaffPos[i] = -1;
  }

  // Определение позиций вставленных нулей
  for (int i = 0; i < BitstaffCodeSize; i++) {
    if (BitstaffCode[i] == 1) {
      OneCount++;
    } else if (BitstaffCode[i] == 0) {
      OneCount = 0;
    }
    if (OneCount == BITSTAFF_LENGTH) {
      BitstaffPos[BitstaffCount] = i + 1;
      BitstaffCount++;
    }
  }

  // Инициализация нового массива, для хранения зашифрованого кода
  int BitstaffDecodeSize = BitstaffCodeSize - (8 - BitstaffCount % 8) - BitstaffCount;
  BitstaffDecode = new int[BitstaffDecodeSize];
  for (int i = 0; i < BitstaffDecodeSize; i++) {
    BitstaffDecode[i] = 0;
  }

  // Шифрование кода
  int i_BitstaffPos = 0;
  int i_BitstaffCode = 0;
  for (int i = 0; i < BitstaffDecodeSize; i++) {
    while (i_BitstaffCode == BitstaffPos[i_BitstaffPos]) {
      i_BitstaffPos++;
      i_BitstaffCode++;
    }
    if (i_BitstaffCode < BitstaffCodeSize) {
      BitstaffDecode[i] = BitstaffCode[i_BitstaffCode];
      i_BitstaffCode++;
    }
  }
  for (int i = 0; i < BitstaffCount; i++) {
    BitstaffDecode[BitstaffDecodeSize - i - 1] = BitstaffCode[BitstaffCodeSize - i - 1];
  }


  // Возврат результата
  return BitstaffDecode;
}
