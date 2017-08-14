/* *******************************************************************************
** usage: spatialmedia [options] [files...]
**
** By default prints out spatial media metadata from specified files.
**
** positional arguments:
**   file                  input/output files
**
** optional arguments:
**   -h, --help            show this help message and exit
**   -i, --inject          injects spatial media metadata into the first file
**                         specified (.mp4 or .mov) and saves the result to the
**                         second file specified
**
** Spherical Video:
**   -s STEREO-MODE, --stereo STEREO-MODE
**                         stereo mode (none | top-bottom | left-right)
**   -c CROP, --crop CROP  crop region. Must specify 6 integers in the form of
**                         "w:h:f_w:f_h:x:y" where w=CroppedAreaImageWidthPixels
**                         h=CroppedAreaImageHeightPixels f_w=FullPanoWidthPixels
**                         f_h=FullPanoHeightPixels x=CroppedAreaLeftPixels
**                         y=CroppedAreaTopPixels
**
** Spatial Audio:
**   -a, --spatial-audio   spatial audio. First-order periphonic ambisonics with
**                        ACN channel ordering and SN3D normalization
**
*********************************************************************************/

#include <algorithm>
#include <iostream>
#include <string.h>

#include <getopt.h>
#include <locale>

#include "parser.h"

using namespace std;

namespace SpatialMedia
{

Parser::Parser ( )
{
  m_bInject       = true;
  m_StereoMode    = SM_NONE;
  m_Projection    = EQUIRECT;
  m_StitchingSoftware = "Spherical Metadata Tool";
  for ( int t=0; t<6; t++ )
    m_crop[t]     = 0;
  m_bSpatialAudio = false;
}

Parser::~Parser ( )
{
}

void Parser::parseCommandLine ( int argc, char *argv[] )
{
  (void)argc;
  (void)argv;

  int iChar = 0;
  static struct option longOpts[] = {
    { "help",          no_argument, 0, 'h' },
    { "inject",        no_argument, 0, 'i' },
    { "projection",  required_argument, 0, 'p' },
    { "stitching-software",  required_argument, 0, 't' },
    { "stereo",  required_argument, 0, 's' },
    { "crop",    required_argument, 0, 'c' },
    { "spatial-audio", no_argument, 0, 'a' },
    { 0, 0, 0, 0 } };

  while ( true )  {
    iChar = getopt_long_only ( argc, argv, "?hias:c:", longOpts, NULL );
    if ( iChar == -1 )
      break;

    switch ( iChar )  {
    case 'i':
      m_bInject = true;
    break;
    case 'a':
      m_bSpatialAudio = true;
    break;
    case 'p':  {
      std::string str ( optarg );
      std::transform ( str.begin ( ), str.end ( ), str.begin ( ), ::tolower );
      if ( str == "equirect" )
        m_Projection = EQUIRECT;
      else if ( str == "cubemap" )
        m_Projection = CUBEMAP;
      else if ( str == "single_fisheye" )
        m_Projection = SINGLE_FISHEYE;
      else
        m_Projection = EQUIRECT;
    }
    break;
    case 's':  {
      std::string str ( optarg );
      std::transform ( str.begin ( ), str.end ( ), str.begin ( ), ::tolower );
      if ( str == "top-bottom" )
        m_StereoMode = SM_TOP_BOTTOM;
      else if ( str == "left-right" )
        m_StereoMode = SM_LEFT_RIGHT;
      else
        m_StereoMode = SM_NONE;
    }
    break;
    case 'c':  {
      std::string str ( optarg ); // w:h:f_w:f_h:x:y => E.g. 2048:1024:3186:1482:100:100
      char* buffer = new char[strlen(optarg)];
      memcpy(buffer, optarg, strlen(optarg));

      char * pch;
      std::string delim(":");
      pch = strtok(buffer, delim.c_str());
      int i = 0;
      std::locale loc;
      while (pch != NULL)
      {
        m_crop[i] = atoi(pch);
        pch = strtok(NULL, delim.c_str());
        i++;
      }

      delete buffer;

      if (i != 6) {
        std::cout << "Error: Invalid crop params: " << optarg << endl;
        exit(0);
      }
    }
    break;
    case 't':  {
      std::string str ( optarg );
      m_StitchingSoftware = str;
    }
    break;

    case 'h':
    case '?':
    default:
      std::cout << iChar << std::endl;
      printHelp ( );
      exit ( 0 );
    }
  }
  if ( argc > 2 )
    m_strInFile = argv[argc-2];
  if ( argc > 3 )
    m_strOutFile = argv[argc-1];
}

void Parser::printHelp ( )
{
  cout << "usage: spatialmedia [options] [files...]" << endl;
  cout << endl;
  cout << "By default prints out spatial media metadata from specified files." << endl;
  cout << endl;
  cout << "positional arguments:" << endl;
  cout << "  file                  input/output files" << endl;
  cout << endl;
  cout << "optional arguments:" << endl;
  cout << "  -h, --help            show this help message and exit" << endl;
  cout << "  -i, --inject          injects spatial media metadata into the first file" << endl;
  cout << "                        specified (.mp4 or .mov) and saves the result to the" << endl;
  cout << "                        second file specified" << endl;
  cout << endl;
  cout << "Spherical Video:" << endl;
  cout << "  -p PROJECTION, --projection PROJECITON" << endl;
  cout << "                        projection type (equirect | cubemap | single-fisheye)" << endl;
  cout << endl;
  cout << "  -t STITCHING_SOFTWARE, --stitching-software STITCHING_SOFTWARE" << endl;
  cout << endl;
  cout << "  -s STEREO-MODE, --stereo STEREO-MODE" << endl;
  cout << "                        stereo mode (none | top-bottom | left-right)" << endl;
  cout << "                        \"none\": Mono frame layout." << endl;
  cout << "                        \"top-bottom\": Top half contains the left eye and bottom half contains the right eye." << endl;
  cout << "                        \"left-right\": Left half contains the left eye and right half contains the right eye." << endl;
  cout << "                        ( RFC: https://github.com/google/spatial-media/tree/master/docs/spherical-video-rfc.md )" << endl;
  cout << endl;
  cout << "  -c CROP, --crop CROP  crop region. Must specify 6 integers in the form of" << endl;
  cout << "                        \"w:h:f_w:f_h:x:y\" where w=CroppedAreaImageWidthPixels" << endl;
  cout << "                        h=CroppedAreaImageHeightPixels f_w=FullPanoWidthPixels" << endl;
  cout << "                        f_h=FullPanoHeightPixels x=CroppedAreaLeftPixels" << endl;
  cout << "                        y=CroppedAreaTopPixels" << endl;
  cout << endl;
  cout << "Spatial Audio:" << endl;
  cout << "  -a, --spatial-audio   spatial audio. First-order periphonic ambisonics with" << endl;
  cout << "                        ACN channel ordering and SN3D normalization" << endl;
  cout << "                        Enables injection of spatial audio metadata. If enabled, the file must contain a" << endl;
  cout << "                        4-channel first-order ambisonics audio track with ACN channel ordering and SN3D" << endl;
  cout << "                        normalization; see the [Spatial Audio RFC](../docs/spatial-audio-rfc.md) for" << endl;
  cout << "                        more information." << endl;

}

std::string &Parser::getInFile ( )
{
  return m_strInFile;
}

std::string &Parser::getOutFile ( )
{
  return m_strOutFile;
}

bool Parser::getInject ( )
{
  return m_bInject;
}

Parser::projection Parser::getProjection ( )
{
  return m_Projection;
}

std::string Parser::getstitchingSoftware ( )
{
  return m_StitchingSoftware;
}

Parser::enMode Parser::getStereoMode ( )
{
  return m_StereoMode;
}

int *Parser::getCrop ( )
{
  // return NULL if no croping was specified.
  for ( int t=0; t<6; t++ )  {
    if ( m_crop[t] != 0 )
      return m_crop;
  }
  return NULL;
}

bool Parser::getSpatialAudio ( )
{
  return m_bSpatialAudio;
}

};  // nd of namespace SpatialMedia
