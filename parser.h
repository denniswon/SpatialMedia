/*****************************************************
** Command line parser class for
** spatial-media
**
**   Created : Sat September 3rd 2016 5:22 pm EST
**        by : Varol Okan
** Copyright : (c) Varol Okan
**   License : LGPL
**
******************************************************/
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>

namespace SpatialMedia
{

class Parser
{
  public:
    typedef enum { SM_NONE, SM_TOP_BOTTOM, SM_LEFT_RIGHT } enMode;
    typedef enum { EQUIRECT, CUBEMAP, SINGLE_FISHEYE } projection;

             Parser ( );
    virtual ~Parser ( );

    void parseCommandLine ( int, char *a[] );

    std::string &getInFile      ( );
    std::string &getOutFile     ( );
    bool         getInject      ( );
    projection   getProjection  ( );
    std::string  getstitchingSoftware ( );
    enMode       getStereoMode  ( );
    int         *getCrop        ( );
    bool         getSpatialAudio( );

  protected:
    void printHelp ( );

  private:
    std::string m_strInFile;
    std::string m_strOutFile;
    bool   m_bInject;
    projection m_Projection;
    enMode m_StereoMode;
    int    m_crop[6];
    std::string m_StitchingSoftware;
    bool   m_bSpatialAudio;
//  -c CROP, --crop CROP  crop region. Must specify 6 integers in the form of
//                        "w:h:f_w:f_h:x:y" where w=CroppedAreaImageWidthPixels
//                        h=CroppedAreaImageHeightPixels f_w=FullPanoWidthPixels
//                        f_h=FullPanoHeightPixels x=CroppedAreaLeftPixels
//                        y=CroppedAreaTopPixels
};


};

#endif // __PARSER_H__
