/*****************************************************************************
 *
 * Copyright 2016 Varol Okan. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

#include <iostream>
#include <stdlib.h>

#include "parser.h"
#include "metadata_utils.h"

using namespace std;

int main ( int argc, char *argv[] )
{
  SpatialMedia::Parser parser;
  parser.parseCommandLine ( argc, argv );
  if ( parser.getInFile ( ) == "" )  {
    cout << "Please provide an input file." << endl;
    exit ( -1 );
  }

  Utils utils;
  if ( parser.getInject ( ) )  {
    if ( parser.getOutFile ( ) == "" )  {
      cout << "Injecting metadata requires both input and output file." << endl;
      exit ( -2 );
    }
    Metadata md;
    std::string &strVideoXML = utils.generate_spherical_xml(
      parser.getProjection(),
      parser.getStereoMode(),
      parser.getstitchingSoftware(),
      parser.getCrop()
    );
    md.setVideoXML ( strVideoXML );
    if ( parser.getSpatialAudio ( ) )
      md.setAudio ( &g_DefAudioMetadata );
    if ( strVideoXML.length ( ) > 1 )
      utils.inject_metadata ( parser.getInFile ( ), parser.getOutFile ( ), &md );
    else
      cout << "Failed to generate metadata." << endl;
  }
  else  {
    utils.parse_metadata ( parser.getInFile ( ) );
  }

  return 0;
}
