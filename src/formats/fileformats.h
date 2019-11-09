#ifndef FILEFORMATS
#define FILEFORMATS

#include <QObject>

class FileFormats {

private:
    bool readonly;

public:

    // Per default enabled image formats
    QStringList formatsQtEnabled;
    QStringList formatsGmEnabled;
//    QStringList formatsExtrasEnabled;

    void setDefaultFormats() {

        formatsQtEnabled.clear();
        formatsGmEnabled.clear();
//        formatsExtrasEnabled.clear();

        /******************************
         ***** 14 FORMATS WORKING *****
         ******************************/

        formatsQtEnabled << "*.bmp"	// Microsoft Windows bitmap
                 << "*.bitmap"
                 << "*.gif"	// CompuServe Graphics Interchange Format
                 << "*.tif"	// Tagged Image File Format
                 << "*.tiff"
                 << "*.jpeg2000"	// JPEG-2000 Code Stream Syntax
                 << "*.ico"	// Microsoft icon
                 << "*.jpeg"	// Joint Photographic Experts Group JFIF format
                 << "*.jpg"
                 << "*.png"	// Portable Network Graphics
                 << "*.pbm"	// Portable bitmap format (black and white)
                 << "*.pgm"	// Portable graymap format (gray scale)
                 << "*.ppm"	// Portable pixmap format (color)
                 << "*.svg"	// Scalable Vector Graphics
                 << "*.xbm"	 // X Windows system bitmap, black and white only
                 << "*.xpm"; // X Windows system pixmap


        formatsGmEnabled << "*.jpc"
                        << "*.j2k"
                           << "*.jpf"
                        << "*.psd"
//                        << "*.tga"
                        << "*.jp2"
                        << "*.icns"
                        << "*.dib"
                        << "*.pcx"
                        << "*.xcf"
                        << "*.svgz"
                        << "*.pnm"
                        << "*.ras"
                        << "*.wbmp"
                        << "*.ani"
                        << "*.eps"
                           ;


#if 0
        formatsExtrasEnabled << "**.psb"
                     << "**.psd"
                     << "**.xcf";


//#ifdef GM


        /**************************************
         ***** 49 FORMATS PASSED THE TEST *****
         **************************************/

// WORKING
        formatsGmEnabled << "*.avs"	//AVS X image
                << "*.x"

// WORKING
                << "*.cals"	// Continuous Acquisition and Life-cycle Support Type 1 image
                << "*.cal"
                << "*.dcl"
                << "*.ras"

// WORKING
                << "*.cin"	// Kodak Cineon

// WORKING
                << "*.cut"	// DR Halo

// WORKING
                << "*.acr"	// Digital Imaging and Communications in Medicine (DICOM) image
                << "*.dcm"
                << "*.dicom"
                << "*.dic"

// WORKING
                << "*.dcx"	// ZSoft IBM PC multi-page Paintbrush image

// WORKING
                << "*.dib"	// Microsoft Windows Device Independent Bitmap

// WORKING
                << "*.dpx"	// Digital Moving Picture Exchange

// WORKING
                << "*.epdf"	// Encapsulated Portable Document Format

// WORKING (Ghostscript required)
                << "*.epi"	// Adobe Encapsulated PostScript Interchange format
                << "*.epsi"

// WORKING (Ghostscript required)
                << "*.eps"	// Adobe Encapsulated PostScript
                << "*.epsf"

// WORKING (Ghostscript required)
                << "*.eps2"	// Adobe Level II Encapsulated PostScript

// WORKING (Ghostscript required)
                << "*.eps3"	// Adobe Level III Encapsulated PostScript

// WORKING (Ghostscript required)
                << "*.ept"	// Adobe Encapsulated PostScript Interchange format with TIFF preview

// WORKING
                << "*.fax"	// Group 3 FAX

// WORKING
                << "*.fits"	// Flexible Image Transport System
                << "*.fts"
                << "*.fit"

// WORKING
                << "*.fpx"	// FlashPix Format

// WORKING
                << "*.jng"	// JPEG Network Graphics

// WORKING
                << "*.mat"	// MATLAB image format

// WORKING
                << "*.miff"	// Magick image file format

// WORKING
                << "*.mono"	// Bi-level bitmap in least-significant-byte first order

// WORKING
                << "*.mtv"	// MTV Raytracing image format

// WORKING
                << "*.otb"	// On-the-air Bitmap

// WORKING
                << "*.p7"	// Xv's Visual Schnauzer thumbnail format

// WORKING
                << "*.palm"	// Palm pixmap

// WORKING
                << "*.pam"	// Portable Arbitrary Map format

// WORKING
                << "*.pcd"	// Photo CD
                << "*.pcds"

// WORKING
                << "*.pcx"	// ZSoft IBM PC Paintbrush file

// WORKING
                << "*.pdb"	// Palm Database ImageViewer Format

// WORKING (Ghostscript required)
                << "*.pdf"	// Portable Document Format

// WORKING
                << "*.pict"	// Apple Macintosh QuickDraw /PICT file
                << "*.pct"
                << "*.pic"

// WORKING
                << "*.pix"	// Alias/Wavefront RLE image format
                << "*.pal"

// WORKING
                << "*.pnm"	// Portable anymap

// WORKING (Ghostscript required)
                << "*.ps"	// Adobe PostScript file

// WORKING (Ghostscript required)
                << "*.ps2"	// Adobe Level II PostScript file

// WORKING (Ghostscript required)
                << "*.ps3"	// Adobe Level III PostScript file

// WORKING
                << "*.psd"	// Adobe Photoshop bitmap file

// WORKING
                << "*.ptif"	// Pyramid encoded TIFF
                << "*.ptiff"

// WORKING
                << "*.sfw"	// Seattle File Works image

// WORKING
                << "*.sgi"	// Irix RGB image

// WORKING
                << "*.sun"	// SUN Rasterfile

// WORKING
//                << "*.tga"	// Truevision Targa image

// WORKING
                << "*.txt"	// Text files

// WORKING
                << "*.vicar"	// VICAR rasterfile format

// WORKING
                << "*.viff"	// Khoros Visualization Image File Format

// WORKING
                << "*.wpg"	// Word Perfect Graphics File

// WORKING
                << "*.xwd"	// X Windows system window dump

// UNTESTED (no test image available)
//#endif
    ;
#endif
    }

    // Read formats from file (if available)
    void getFormats()
    {
        setDefaultFormats();
//        QFile file(QDir::homePath() + "/.nfspb/fileformats.disabled");
    }

};




#endif // FILEFORMATS

