/*
 * TinyJS
 *
 * A single-file Javascript-alike engine
 *
 * Authored By Gordon Williams <gw@pur3.co.uk>
 *
 * Copyright (C) 2009 Pur3 Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * This is a program to run all the tests in the tests folder...
 */

#include "TinyJS.h"
#include "TinyJS_Functions.h"
#include "TinyJS_MathFunctions.h"
#include <assert.h>
#include <sys/stat.h>
#include <string>
#include <sstream>
#include <stdio.h>
#include <iostream>

#ifdef MTRACE
  #include <mcheck.h>
#endif

using namespace std;

/*int run_command(char command) {
  CTinyJS s;
  registerFunctions(&s);
  registerMathFunctions(&s);
  s.root->addChild("result", new CScriptVar("0",SCRIPTVAR_INTEGER));
  try {
    s.execute(&command);
  } catch (CScriptException *e) {
    printf("ERROR: %s\n", e->text.c_str());
  }
  CScriptVar *pass = s.root->getParameter("result");
  string result = pass->getString();

  if (pass->getBool())
    printf("%s \n", result.c_str());
  else {
    char fn[64];
    sprintf(fn, "\"%s\"", &command);
    FILE *f = fopen(fn, "wt");
    if (f) {
      std::ostringstream symbols;
      s.root->getJSON(symbols);
      fprintf(f, "%s", symbols.str().c_str());
      fclose(f);
    }

    printf("Executing the js-command %s failed.\n", fn);
  }
  return 0;
}

int run_console() {
  while (true)
  {
    char consoleInput;
    cin >> consoleInput;
    int ret = run_command(consoleInput);
    if (ret == 0)
    {
      continue;
    } else if (ret == 1)
    {
      break;
    } else {
      return 1;
    }
    
  }
  return 0;
  
}*/

bool run_test(const char *filename) {
  printf("Executing %s. The result is: \n", filename);
  struct stat results;
  if (!stat(filename, &results) == 0) {
    printf("Cannot stat file! '%s'\n", filename);
    return false;
  }
  int size = results.st_size;
  FILE *file = fopen( filename, "rb" );
  /* if we open as text, the number of bytes read may be > the size we read */
  if( !file ) {
     printf("Unable to open file! '%s'\n", filename);
     return false;
  }
  char *buffer = new char[size+1];
  long actualRead = fread(buffer,1,size,file);
  buffer[actualRead]=0;
  buffer[size]=0;
  fclose(file);

  CTinyJS s;
  registerFunctions(&s);
  registerMathFunctions(&s);
  s.root->addChild("result", new CScriptVar("0",SCRIPTVAR_INTEGER));
  try {
    s.execute(buffer);
  } catch (CScriptException *e) {
    printf("ERROR: %s\n", e->text.c_str());
  }
  bool pass = s.root->getParameter("result")->getBool();
  string result = s.root->getParameter("result")->getString();

  if (pass)
    printf("%s \n", result.c_str());
  else {
    char fn[64];
    sprintf(fn, "%s.fail.js", filename);
    FILE *f = fopen(fn, "wt");
    if (f) {
      std::ostringstream symbols;
      s.root->getJSON(symbols);
      fprintf(f, "%s", symbols.str().c_str());
      fclose(f);
    }

    printf("Executing the js-command %s failed.\n", fn);
  }

  delete[] buffer;
  return pass;
}

int main(int argc, char **argv)
{
  printf("TinyJS console\n");
  printf("USAGE:\n");
  printf("   ./console file.js       : run the code of a file\n");
  if (argc==2) {
    return !run_test(argv[1]);
  } else if (argc==1) {
    printf("You have to give one filepath as an argument to run");
  } else
  {
    for (int i = 1; i < argc; i++)
    {
      run_test(argv[i]);
    }
  }

  return 0;
}
