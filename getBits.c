/********************************************************/
/*Ronald Espinoza                                       */
/*2/20/2020                                             */
/*CS-241 Section 005                                    */
/*                                                      */
/*Purpose:                                              */
/*  This program reads a record from the standard input */
/*  stream, passes the data in each valid record to     */
/*  getbits and displays the result                     */
/*How to Use this source file:                          */
/*  This program compiles using gcc -o getBits getBits.c*/
/*  and can be run using ./getBits input.txt            */
/*  This will produce a display for the results of each */
/*  record that exists in the file. See examples below  */
/*127;32;4                                              */
/*127;2;34                                              */
/*4295000000;5;1                                        */
/*NOTE:  6.6 (6) [C99,n1570 draft of the C2011 standard]*/
/*  Switch labels must be constant expressions, they    */
/*  have to be evaluated at compile time. If you want to*/
/*  branch on run-time values, you must use an if.      */
/********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <limits.h>

/*global definitions*/
#define BASE_TEN_START  48
#define BASE_TEN_END    57
#define SEMICOLON       59
#define CARRIAGE_RETURN 13
#define NEW_LINE        10
#define WORD_SIZE       32
#define POSITION_INDEX  1
#define SHIFT_INDEX     2
#define DIGIT_RANGE     10
#define ERROR_CODE      10100101

/*Error Messages*/
#define E_1  "Error: no File name was provided!\n"
#define E_2  "Error: there is no data in File!\n"
#define E_3  "Error: Invalid Character in record\n"
#define E_4  "Error: Record was empty\n"
/**** stack smashing detected ***: heap has no room*/
#define E_5  "Error: too many bits requested from position\n"
/*if right hand operand of shift is greater than or*/
/*equal to the number of bits of the type of the left*/
/*hand operand the behavior is undefined*/
#define E_6  "Error: position out of range\n"
#define E_7  "Error: number of bits out of range\n"
#define E_8  "Error: value out of range\n"
/*should never happen due to range check*/
#define E_9  "Error: STRTOL returned a null value!\n"
#define E_10 "Error: Invalid start to record!\n"
/*There will be no change if there is no shift size*/
#define E_11 "Error: Shift size is Zero\n"
#define E_12 "Error: The bits smashed the stack\n"
#define E_13 "Error: Record is missing Semicolon\n"
#define E_14 "Error: Position is missing!\n"
#define E_15 "Error: Shift Size is missing!\n"
#define E_16 "Error: Something horrible happened in main!\n"

/********************************************************/
/*Name: getBits                                         */
/*Params: unsigned long int theFields[3]                */
/*Datatype:Int, input or output: output                 */
/*  - Def: getbits: get n bits from position p          */
/*         Section 2.9 of Kernighan and Ritchie         */
/*         return (x >> (p+1-n)) & ~(~0 << n);          */
/*  - range of values: 0 -> (+) Infinity                */
/*Functions' return value: the (right adjusted) n-bit   */
/*                         field of x                   */
/*Description:                                          */
/*This function returns a positive integer according to */
/*  the values retrieved in the current record          */
/*Functions' Algorithm                                  */
/*set the position and shiftSize variables              */
/*get the shiftLeft value                               */
/*  shift n and place zeros in right most bit           */
/*  mask this new n with 1 in the rightmost n bits      */
/*get the shiftPosition from p + 1 - n                  */
/*Then shift the bits to the right shift position       */
/*Mask the shift right value from the result            */
/*  in the shift left variable                          */
/*return the processed bits                             */
/********************************************************/
unsigned int getBits(unsigned long int theFields[3])
{/*split param theFields[] to local variables*/
  unsigned int theBits = theFields[0];
  int position = theFields[1];
  int shiftSize = theFields[2];
  unsigned int shiftLeft = 0;
  unsigned int shiftRight = 0;
  unsigned int shiftPosition = 0;
  /*mask the shift size with ones*/
  shiftLeft = ~(~0 << shiftSize);
  shiftPosition = position + 1 - shiftSize;
  /*shift the bits right to the new position*/
  shiftRight = theBits >> shiftPosition;
  theBits = shiftLeft & shiftRight;
  return theBits;
}

/********************************************************/
/*Name: fileOpened                                      */
/*Params:  FILE *fp                                     */
/*Datatype:bool, input or output: output                */
/*  - Def: check existence of file pointer              */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a file?             */
/*Description:                                          */
/*This function returns true if there is a file         */
/*Functions' Algorithm                                  */
/*if(file point doesn't exist)                          */
/*  return there was no file                            */
/*else                                                  */
/*  default return, there was a file                    */
/********************************************************/
bool fileOpened(FILE *fp)
{
  if(fp == NULL)
  {/*check if pointer has a value*/
    return false;
  }
  else
  {/*default to true for all fp values != 0*/
    return true;
  }
  
}

/********************************************************/
/*Name: checkTerminalInput                              */
/*Params: int arg_count                                 */
/*Datatype:Void, input or output: input                 */
/*  - Def: check if there was filename supplied         */
/*  - range of values: any characters after ./a.out '---*/
/*Functions' Data-Type: Void                            */
/*Functions' return value: N/A                          */
/*Description:                                          */
/*This function exits the program if there was no file  */
/*  name provided                                       */
/*Functions' Algorithm                                  */
/*if(file name doesn't exist)                           */
/*  print error message                                 */
/*  exit program as there are no records                */
/*else                                                  */
/*  default return, there was a file name               */
/********************************************************/
void checkTerminalInput(int arg_count)
{
  /* in.txt makes the arg_count = 2*/
  if(arg_count != 2)
  {/*"Error: No File name was provided!\n"*/
    printf(E_1);
    exit(1);
  }
  return;
}

/********************************************************/
/*Name:isSemiColon                                      */
/*Params: char currChar                                 */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if current character is a semicolon(;) */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a semicolon?        */
/*Description:                                          */
/*This function returns true if the character recieved  */
/*  is a semicolon(;)                                   */
/*Functions' Algorithm                                  */
/*if(char is semicolon)                                 */
/*  return found semicolon                              */
/*else                                                  */
/*  default return, there was no semicolon              */
/********************************************************/
bool isSemiColon(char currChar)
{
  if(currChar == SEMICOLON)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/********************************************************/
/*Name:isNewLine                                        */
/*Params: char currChar                                 */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if current character is a new line feed*/
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a new line feed?    */
/*Description:                                          */
/*This function returns true if the character recieved  */
/*  is a new line feed                                  */
/*Functions' Algorithm                                  */
/*if(char is new line or carriage return)               */
/*  return found new line feed                          */
/*else                                                  */
/*  default return, there was no new line feed          */
/********************************************************/
bool isNewLine(char currChar)
{/*to check the end of the records*/
  if(/*this is because the file end lines with CRLF*/
      (currChar == NEW_LINE) ||
      (currChar == CARRIAGE_RETURN)
    )
  {
    return true;
  }
  else if(currChar == '\0')
  {/*happens at the last line or EOF*/
    return true;
  }
  else
  {/*default to not a new line character*/
    return false;
  }
}

/********************************************************/
/*Name:isBaseTen                                        */
/*Params: char currChar                                 */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if current character is base ten (0-9) */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a base ten value?   */
/*Description:                                          */
/*This function returns true if the character recieved  */
/*  is a base ten value                                 */
/*Functions' Algorithm                                  */
/*if(char is base ten value)                            */
/*  return found base ten value                         */
/*else                                                  */
/*  default return, there was no base ten value         */
/********************************************************/
bool isBaseTen(char currChar)
{
  if(/*condition for base ten (0-9) ASCII int values*/
      (currChar >= BASE_TEN_START) &&
      (currChar <= BASE_TEN_END)
    )
  {/*occurs when currChar is a digit 0-9*/
    return true;
  }
  else
  {/*default is not a base ten character*/
    return false;
  }
}

/********************************************************/
/*Name:isValidCharacter                                 */
/*Params: char currChar                                 */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if current character is valid          */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a valid character?  */
/*Description:                                          */
/*This function returns true if the character recieved  */
/*  is '(0-9)', ';', 'CR', 'LF', or '\0'                */
/*Functions' Algorithm                                  */
/*if(char is is in range)                               */
/*  return found a valid character                      */
/*else                                                  */
/*  default return, there was no valid character        */
/********************************************************/
bool isValidCharacter(char currChar)
{
  if(/*a valid record is ##;##;##(CRLF or \0)*/
      isBaseTen(currChar) ||
      isSemiColon(currChar) ||
      isNewLine(currChar)
    )
  {/*when currChar is a valid char for a valid record*/
    return true;
  }
  else
  {/*default not a valid character or record*/
    return false;
  }
  
}

/********************************************************/
/*Name:getBaseTenInt                                    */
/*Params: char currChar                                 */
/*Datatype:int, input or output: output                 */
/*  - Def: convert char as int to its' true int value   */
/*  - range of values: 0-9                              */
/*Functions' return value: the true int of recived char */
/*Description:                                          */
/*This function gets the ASCII int value for the current*/
/*  character                                           */
/*Functions' Algorithm                                  */
/*get current character as int then subtract by ASCII   */
/*  base ten start                                      */
/*return the current character as its' integer value    */
/********************************************************/
int getBaseTenInt(char currChar)
{/*logic was from ascii table i.e. '1' = 49 - 48 = 1*/
  int theIntValue = 0;
  /*control data type of currChar to supress warning*/
  theIntValue = (int) currChar - BASE_TEN_START;
  return theIntValue;
}

/********************************************************/
/*Name: getParamAsInt                                   */
/*Params: char theCurrentParam[]                        */
/*Datatype:Unsigned long int, input or output: output   */
/*  - Def: convert the parameter to its' integer value  */
/*  - range of values: 0 -> (+) Infinity                */
/*Functions' return value: integer value of the param   */
/*Description:                                          */
/*This function returns a positive integer according to */
/*  the value recieved as the current parameter         */
/*Functions' Algorithm                                  */
/*set paramAsInt to the strings value created from      */
/*  theCurrentParam stored in param temp as a base ten  */
/*  number. STROL is used because of its' return types  */
/*if(param is not a decimal number)                     */
/*  print and return an error code                      */
/*else                                                  */
/*  return the current parameters integer value         */
/********************************************************/
unsigned long int getParamAsInt(char theCurrentParam[])
{
  unsigned long int paramAsInt = 0;
  char *paramTemp = " ";
  /*strtol( StringToConvert, &pointerAddress, Base(2-32)*/
  paramAsInt = strtol(theCurrentParam, &paramTemp, 10);
  if(/*no conversion could be performed*/
      (paramTemp == theCurrentParam) &&
      !isNewLine(paramAsInt)
    )/*the strings will be equivalent*/
  {/*"Error: STRTOL returned a null value!\n"*/
    printf(E_9);
    return ERROR_CODE;
  }
  else
  {/*return newly converted integer*/
    return paramAsInt;
  }
}

/********************************************************/
/*Name:smashedStack                                     */
/*Params: int params[2], char theParam[]                */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if param as int smashes allocated stack*/
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is there a smashed stack?    */
/*Description:                                          */
/*This function returns true if the param as an integer */
/*  smashed the stack or range allocated for it         */
/*Functions' Algorithm                                  */
/*if(the bits as int is greater than the range)         */
/*  return stack is smashed                             */
/*else if(the position has too many digits)             */
/*  return stack is smashed                             */
/*else if(the shift size has too many digits)           */
/*  return stack is smashed                             */
/*else                                                  */
/*  default return, the param stack is not smashed      */
/********************************************************/
bool smashedStack(int params[2], char theParam[])
{
  int paramCount = params[0];
  int paramDigits = params[1];
  unsigned long int theParamAsInt = 0;
  /*stray \342 error occurs w/out sign*/
  /*max base ten value for word Size*/
  const unsigned long int bitRange = UINT_MAX;
  theParamAsInt = getParamAsInt(theParam);

  /*Prevent stack smashing above WORD_SIZE digits*/
  /*‭1111-1111-1111-1111-1111-1111-1111-1111*/
  /*‭UINT_MAX‬ - which is ten digits*/
  if(theParamAsInt > bitRange)
  {
    return true;
  }
  else if(/*check if position will smash it's stack*/
      (paramCount == 1) &&
      (paramDigits > DIGIT_RANGE)
    )
  {/*"Error: position out of range\n"*/
    printf(E_6);
    return true;
  }
  else if(/*check if Number of bits will smash the stack*/
      (paramCount == 2) &&
      (paramDigits > DIGIT_RANGE)
    )
  {/*"Error: number of bits out of range\n"*/
    printf(E_7);
    return true;
  }
  else
  {
    return false;
  }
}

/********************************************************/
/*Name:badRecordStart                                   */
/*Params: char buffStart                                */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if buffer start is valid               */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: Does buff have a valid start?*/
/*Description:                                          */
/*This function returns true if the character recieved  */
/*  is '(0-9)'                                          */
/*Functions' Algorithm                                  */
/*if(buffer start is not base ten)                      */
/*  print and return an invalid start to record         */
/*else if(buffer start is a new line feed)              */
/*  print and return an invalid start to record         */
/*else                                                  */
/*  default return, there was a valid start for buff    */
/********************************************************/
bool badRecordStart(char buffStart)
{
  if(isBaseTen(buffStart))
  {/*start with 0-9*/
    return false;
  }
  else if(isNewLine(buffStart))
  {/*blank new line*/
    return true;
  }
  else if(isSemiColon(buffStart))
  {/*there are no bits*/
    return true;
  }
  else
  {/*we had a valid starting character*/
    return false;
  }
}

/********************************************************/
/*Name:listHasErrorCode                                 */
/*Params: unsigned long int paramList[3]                */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if paramList contains ERROR_CODE       */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: Was there an error code?     */
/*Description:                                          */
/*This function returns true if there was no error code */
/*Functions' Algorithm                                  */
/*See if there was a flag set when building the list    */
/*if shift size is an error code                        */
/*  print and return invalid character in record        */
/*else if position is an error code                     */
/*  print and return the bits smashed the stack         */
/*else if shift size and position are error codes       */
/*  print and return record is missing semicolon        */
/*else if the bits are an error code                    */
/*  print and return shift size was zero                */
/*else                                                  */
/*  default return, there was no error code in the list */
/********************************************************/
bool listHasErrorCode(unsigned long int paramList[3])
{
  if(
    paramList[2] == ERROR_CODE &&
    paramList[1] == 0 &&
    paramList[0] == 0 
    )
  {/*"Error: Invalid Character in record\n"*/
    printf(E_3);
    return true;
  }
  else if(
    paramList[2] == 0 &&
    paramList[1] == ERROR_CODE &&
    paramList[0] == 0 
    )
  {/*"Error: The bits smashed the stack\n"*/
    printf(E_12);
    return true;
  }
  else if(
    paramList[2] == ERROR_CODE &&
    paramList[1] == ERROR_CODE &&
    paramList[0] == 0 
    )
  {/*"Error: Record is missing Semicolon\n"*/
    printf(E_13);
    return true;
  }
  else if(
    paramList[2] == 0 &&
    paramList[1] == 0 &&
    paramList[0] == ERROR_CODE 
    )
  {/*"Error: Shift size is Zero\n"*/
    printf(E_11);
    return true;
  }
  else if(
    paramList[2] == ERROR_CODE &&
    paramList[1] == 0 &&
    paramList[0] == ERROR_CODE 
    )
  {/*invalid start to record*/
    printf(E_10);
    return true;
  }
  else
  {/*default no error code in list*/
    return false;
  }
}


/********************************************************/
/*Name:checkBitRequest                                  */
/*Params: unsigned long int paramList[3]                */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if bits are in range                   */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: Are the bits in range?       */
/*Description:                                          */
/*This function returns true if the bits requested are  */
/*  in range                                            */
/*Functions' Algorithm                                  */
/*See if there was a flag set when building the list    */
/*if(list had an error code)                            */
/*  return the bit request was invalid                  */
/*else if(position is out of range or missing)          */
/*  print and return associated position error          */
/*else if(shift size is out of range or missing)        */
/*  print and return associated shift size error        */
/*else if(buffer start is a new line feed)              */
/*  print and return an invalid start to record         */
/*else if(result is undefined)                          */
/*  return bit request is invalid                       */
/*else                                                  */
/*  default return, there was a valid bit request       */
/********************************************************/
bool checkBitRequest(unsigned long int paramList[3])
{
  if(listHasErrorCode(paramList))
  {/*flags set when building paramList*/
    return false;
  }
  else if(paramList[1] >= WORD_SIZE)
  {/*"Error: position out of range\n"*/
    printf(E_6);
    return false;
  }
  else if(paramList[1] == '\0')
  {/*"Error: Position is missing!\n"*/
    printf(E_14);
    return false;
  }
  else if(paramList[2] >= WORD_SIZE)
  {/*"Error: number of bits out of range\n"*/
    printf(E_7);
    return false;
  }
  else if(paramList[2] == '\0')
  {/*"Error: Shift Size is missing\n"*/
    printf(E_15);
    return false;
  }
  else if(paramList[2] > paramList[1] + 1)
  {/*"Error: too many bits requested from position\n"*/
    printf(E_5);
    return false;
  }
  else
  {/*default bits requested are in range*/
    return true;
  }
}

/********************************************************/
/*Name:doesShiftExist                                   */
/*Params: int shiftSize                                 */
/*Datatype:Bool, input or output: output                */
/*  - Def: check if shift is not zero                   */
/*  - range of values: TRUE or FALSE                    */
/*Functions' return value: is shift size zero?          */
/*Description:                                          */
/*This function returns true if the shift size is > 0   */
/*Functions' Algorithm                                  */
/*if(shift size is greater than zero)                   */
/*  return a shift does exist                           */
/*else                                                  */
/*  default return, there was no shift size             */
/********************************************************/
bool doesShiftExist(int shiftSize)
{
  if(shiftSize > 0)
  {/*The getbits function will work properly*/
    return true;
  }
  else
  {/*nothing will be shifted*/
    return false;
  }
}

/********************************************************/
/*Name: buildParamList                                  */
/*Params: char buff[]                                   */
/*Datatype:Unsigned long int *, input or output: output */
/*  - Def: build list of params from record in buff     */
/*  - range of values: [(0-range), (0-range), (0-range)]*/
/*                      range = UINT_MAX                */
/*Functions' return value: parameters in record         */
/*Description:                                          */
/*This function returns a pointer for an array of ints  */
/*  that are collected as a list from the parameters    */
/*  in the record                                       */
/*Functions' Algorithm                                  */
/*while(going through all characters in the buff)       */
/*  if current character is not valid                   */
/*    set shift size to error code                      */
/*  if current character is base ten                    */
/*    if current parameter smashes its' stack           */
/*      set position to error code                      */
/*    else continue to append to the parameter          */
/*  else if current character is the string delimeter   */
/*    allocate element to new index in parameter list   */
/*  else if current character is a new line feed        */
/*    if not all params were collected                  */
/*    set shift size and position to error code         */
/*    set the last element in the parameter list        */
/*    then leave                                        */
/*if shift size is zero set the bit to an error code    */
/*else return the paramter list                         */
/********************************************************/
unsigned long int * buildParamList(char buff[])
{
  char theParam[WORD_SIZE] = " ";
  int paramCount = 0;
  int paramDigits = 0;
  int charCount = 0;
  int params[2] = {0, 0};
  static unsigned long int paramList[3] = {0, 0, 0};

  if(badRecordStart(buff[0]))
  {/*leave function as no processing can be done*/
    paramList[0] = ERROR_CODE;
    paramList[1] = 0;
    paramList[2] = ERROR_CODE;
    return paramList;
  }

  while(charCount < 128)
  {/*get the characters in the line*/
    char currChar = buff[charCount];
    if(!isValidCharacter(currChar))
    {
      paramList[0] = 0;
      paramList[1] = 0;
      paramList[2] = ERROR_CODE;
      return paramList;
    }
    if(isBaseTen(currChar))
    {/*if character is integer then appened value*/
      paramDigits++;
      params[0] = paramCount;
      params[1] = paramDigits;
      if(smashedStack(params, theParam))
      {
        paramList[0] = 0;
        paramList[1] = ERROR_CODE;
        paramList[2] = 0;
        return paramList;
      }
      strncat(theParam, &currChar, 1);
    }
    else if(isSemiColon(currChar))
    {/*the identifier that allows us to split the param*/
      paramList[paramCount] = getParamAsInt(theParam);
      theParam[0] = '\0';
      paramCount++;
      paramDigits = 0;
    }
    else if(isNewLine(currChar))
    {/*Occurs at the last param (shiftSize)*/
      /*missing 1 ; will be paramCount at 1*/
      /*missing 2 ; will be paramCount at 0*/
      if(paramCount == 0 || paramCount == 1)
      {
        paramList[0] = 0;
        paramList[1] = ERROR_CODE;
        paramList[2] = ERROR_CODE;
        return paramList;
      }
      else
      {/*the record parameters were valid*/
        paramList[paramCount] = getParamAsInt(theParam);
        paramDigits = 0;
        paramCount = 0;
        break;
      }
    }
    charCount++;
  }/*paramList = {theBits(x),position(p),shiftSize(n)}*/
  if(doesShiftExist(paramList[2]))
  {
    return paramList;
  }
  else
  {
    paramList[0] = ERROR_CODE;
    paramList[1] = 0;
    paramList[2] = 0;
    return paramList;
  }
}

/********************************************************/
/*Name: readRecord                                      */
/*Params: char buff[]                                   */
/*Datatype:Void, input or output: input                 */
/*  - Def: read the record and print the result         */
/*  - range of values: N/A                              */
/*Functions' Data-Type: Void                            */
/*Functions' return value: N/A                          */
/*Description:                                          */
/*This function gets and prints the records' result     */
/*Functions' Algorithm                                  */
/*get the parameter list from the record                */
/*then check if our parameters are valid                */
/*  and get the bits from getBits                       */
/*  then display the result                             */
/*else                                                  */
/*  print that our result was undefined                 */
/********************************************************/
void readRecord(char buff[])
{
  /*theBits; position; shiftSize*/
  unsigned int theBitsRecieved = 0;
  /*stray \342 error occurs without sign*/
  const unsigned long int bitRange = UINT_MAX;
  unsigned long int *paramList;
  paramList = buildParamList(buff);
  if(paramList[0] > bitRange)
  {/*"Error: value out of range\n"*/
    printf(E_8);
    return;
  }
  if(checkBitRequest(paramList)){
    /*in a valid record collect theBits*/
    /*from the param list built*/
    theBitsRecieved = getBits(paramList);
    /*default to printing records' result*/
    printf("getbits(x=%ld, ", paramList[0]);
    printf("p=%ld, ", paramList[1]);
    printf("n=%ld) ", paramList[2]);
    printf("= %d\n", theBitsRecieved);
    return;
  }
}

/********************************************************/
/*Name: main                                            */
/*Params:int arg_count, char *file_name[]               */
/*Datatype:Int, input or output: input                  */
/*  - Def: main entry point for input to program        */
/*  - range of values: 0,1                              */
/*Functions' Data-Type: int                             */
/*Functions' return value: 1 if something bad occurs    */
/*Description:                                          */
/*This function opens and reads the records from a file */
/*Functions' Algorithm                                  */
/*if there was a file name supplied open the file       */
/*See if the file opened                                */
/*  while reading the file till the end of the file     */
/*    read the records in the file                      */
/*else there was no data in the file                    */
/*and mains return will be in an error state            */
/********************************************************/
int main(int arg_count, char *file_name[])
{
  int mainsReturn = 1;
  FILE *fp;
  char buff[128];
  /*validate an input file was supplied*/
  checkTerminalInput(arg_count);
  fp = fopen(file_name[1], "r");/*read the file*/
  if(fileOpened(fp))
  {/*loop through the text in the file till EOF*/
    while(fgets(buff, sizeof(buff), fp) != NULL)
    {
      readRecord(buff);
    }/*all code executed properly*/
    mainsReturn = 0;
  }
  else
  {/*"Error: There is no data in File!\n"*/
    printf(E_2);
    mainsReturn = 1;
  }
  /*release the file*/
  fclose(fp);
  /*check for catastrophic failure in main*/
  if(mainsReturn == 0)
  {
    return mainsReturn;
  }
  else
  {/*"Error: Something horrible happened in main!\n"*/
    printf(E_16);
    return 1;
  }
}