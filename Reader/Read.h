//  Powiter
//
//  Created by Alexandre Gauthier-Foichat on 06/12
//  Copyright (c) 2013 Alexandre Gauthier-Foichat. All rights reserved.
//  contact: immarespond at gmail dot com
#ifndef __READ__H__
#define __READ__H__

#include <QtCore/QString>
#include <QtCore/QChar>
#include <map>
#include <QtCore/QStringList>
#include "Core/channels.h"
#include "Core/row.h"
#include "Core/displayFormat.h"
#include "Superviser/PwStrings.h"
class ViewerGL;
class Reader;
class Lut;
class ReaderInfo;
class Read{
    
public:
	Read(Reader* op,ViewerGL* ui_context);
	virtual ~Read();
    
    /*This must be implemented to do the to linear colorspace conversion*/
	virtual void engine(int y,int offset,int range,ChannelMask channels,Row* out)=0;
    
    /*can be overloaded to add knobs dynamically to the reader depending on the file type*/
	virtual void createKnobDynamically();
    
    /*Must be implemented to tell whether this file type supports stereovision*/
	virtual bool supports_stereo()=0;
    
//    /*Must be implemented. This function opens the file specified by filename, reads
//     all the meta-data and extracts all the data. If openBothViews is on, this function
//     should read both views of the file.
//     More precise documentation coming soon.*/
//    virtual void open(const QString filename,bool openBothViews = false)=0;
    
    /*This function open calls readHeader and depending whether the Read*
     supports scan line reading, it calls either readAllData or just readScanLine*/
    void open(const QString filename,bool fitFrameToviewer,bool openBothViews = false);
    
    /*Should open the file and call setReaderInfo with the infos from the file.*/
    virtual void readHeader(const QString filename,bool openBothViews)=0;
    
    /*Must be implemented to know whether this Read* supports reading only
     scanlines. In the case it does,the engine can be must faster by reading
     only the lines that are needed.*/
    virtual bool supportsScanLine()=0;
    
    /*Must be overloaded by reader that supports scanline. It should return
     the current count of scanlines read by the Read* */
    virtual int scanLinesCount(){return 0;}
    
    /*Should read the already opened file and extract all the data for either 1 view or
     2 views. By default does nothing, you should either overload this function or readScanLine.*/
    virtual void readAllData(bool openBothViews){}
    
    /*Must be implemented if supportsScanLine() returns true. It should
     read one scan line from the file.By default does nothing, you should
     either overload this function or readScanLine*/
    virtual void readScanLine(int y){}
    
    /*Must be overloaded: this function is used to create a little preview that
     can be seen on the GUI node*/
    virtual void make_preview(const char* filename)=0;
    
    /*Returns true if the file is stereo*/
    bool fileStereo(){return is_stereo;};
    
    /*Returns true if the file has alpha premultiplied data*/
    bool premultiplied(){return _premult;}
    
    /*Returns true if the user checked autoAlpha:
     this feature creates automatically an empty alpha channel*/
    bool autoAlpha(){return _autoCreateAlpha;}
    
    /*Returns the reader colorspace*/
    Lut* lut(){return _lut;}
    
    /*This function should be call at the end of open(...)
     It set all the reader infos necessary for the read frame.*/
	void setReaderInfo(Format dispW,
                       Box2D dataW,
                       QString file,
                       ChannelMask channels,
                       int Ydirection ,
                       bool rgb );
    
    /*Returns all the infos necessary for the current frame*/
    ReaderInfo* getReaderInfo(){return _readInfo;}
    
protected:
    
    void from_byte(float* r,float* g,float* b, const uchar* from, bool hasAlpha, int W, int delta ,float* a,bool qtbuf = true);
   
    void from_short(float* r,float* g,float* b, const U16* from, const U16* alpha, int W, int bits, int delta ,float* a);
   
	void from_float(float* r,float* g,float* b, const float* fromR,const float* fromG,
                    const float* fromB, int W, int delta ,const float* fromA,float* a);
    
        
	bool is_stereo;
    bool _premult; //if the file contains a premultiplied 4 channel image, this must be turned-on
    bool _autoCreateAlpha;
	Reader* op;
    ViewerGL* ui_context;
    Lut* _lut;
	ReaderInfo* _readInfo;
    
};









#endif // __READ__H__