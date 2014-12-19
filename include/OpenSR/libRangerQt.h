#ifndef LIBRANGER_QT_H
#define LIBRANGER_QT_H

#include <stdint.h>

#include <QImage>

class QIODevice;

namespace OpenSR
{
//! Header of layer in *.gi files
struct GILayerHeader
{
    uint32_t seek;  //!< Layer offset in file
    uint32_t size;  //!< Layer size
    uint32_t startX;  //!< Layer left corner
    uint32_t startY;  //!< Layer top corner
    uint32_t finishX;  //!< Layer rigth corner
    uint32_t finishY;  //!< Layer bottom corner
    uint32_t unknown1;
    uint32_t unknown2;
};

//! Header of frame in *.gi file
struct GIFrameHeader
{
    uint32_t signature;  //!< Signature
    uint32_t version;  //!< Version of GI file
    uint32_t startX;  //!< Left corner
    uint32_t startY;  //!< Top corner
    uint32_t finishX;  //!< Right corner
    uint32_t finishY;  //!< Bottom corner
    uint32_t rBitmask;  //!< Mask of r color component
    uint32_t gBitmask;  //!< Mask of g color component
    uint32_t bBitmask;  //!< Mask of b color component
    uint32_t aBitmask;  //!< Mask of a color component
    uint32_t type;  //!< Frame type
    /*!<
     * Variants:
     *  -# 0 - One layer, 16 or 32 bit, depends on mask.
     *  -# 1 - One layer, 16 bit RGB optimized.
     *  -# 2 - Three layers:
     *   -# 16 bit RGB optimized - body
     *   -# 16 bit RGB optimized - outline
     *   -# 6 bit Alpha optimized
     *  -# 3 - Two layers:
     *   -# Indexed RGB colors
     *   -# Indexed Alpha
     *  -# 4 - One layer, indexed RGBA colors
     *  -# 5 - Delta frame of GAI animation.
     */
    uint32_t layerCount; //!< Number of layers in frame
    uint32_t unknown1;
    uint32_t unknown2;
    uint32_t unknown3;
    uint32_t unknown4;
};
const uint32_t GI_FRAME_SIGNATURE = 0x00006967;

//! Header of animation in *.gai file
struct GAIHeader
{
    uint32_t signature;  //!< File signature
    uint32_t version;  //!< Format version
    uint32_t startX;  //!< Left corner
    uint32_t startY;  //!< Top corner
    uint32_t finishX;  //!< Right corner
    uint32_t finishY;  //!< Bottom corner
    uint32_t frameCount; //!< Number of frames in animation
    uint32_t haveBackground; //!< Animation a background in separate file
    uint32_t waitSeek;  //!< Wait seek?
    uint32_t waitSize;  //!< Wait size?
    uint32_t unknown1;
    uint32_t unknown2;
};
const uint32_t GAI_SIGNATURE = 0x00696167;

struct HAIHeader
{
    uint32_t signature;  //!< Signature
    uint32_t width;  //!< Animation width
    uint32_t height;  //!< Animation height
    uint32_t rowBytes;  //!< Bytes in one line
    uint32_t count;  //!< Number of frames in animation
    uint32_t frameSize;  //!< Size of one frame
    uint32_t unknown1;
    uint32_t unknown2;
    uint32_t unknown3;
    uint32_t unknown4;
    uint32_t unknown5;
    uint32_t unknown6;
    uint32_t palSize;  //!< Size of pallete
};
const uint32_t HAI_SIGNATURE = 0x04210420;
//FIXME: Check this
const int HAI_FRAME_TIME = 50;

struct Animation
{
    QVector<QImage> images;
    QVector<int> times;
};

bool checkHAIHeader(QIODevice *dev);
HAIHeader peekHAIHeader(QIODevice *dev);
HAIHeader readHAIHeader(QIODevice *dev);

QVector<int> loadHAITimes(QIODevice *dev, const HAIHeader& header);
QImage loadHAIFrame(QIODevice *dev, const HAIHeader& header, int i);

Animation loadHAIAnimation(QIODevice *dev);

bool checkGAIHeader(QIODevice *dev);
GAIHeader peekGAIHeader(QIODevice *dev);
GAIHeader readGAIHeader(QIODevice *dev);

QVector<int> loadGAITimes(QIODevice *dev, const GAIHeader& header);
QImage loadGAIFrame(QIODevice *dev, const GAIHeader& header, int i, const QImage &background);

Animation loadGAIAnimation(QIODevice *dev, const QImage &background = QImage());

bool checkGIHeader(QIODevice *dev);
GIFrameHeader peekGIHeader(QIODevice *dev);
GIFrameHeader readGIHeader(QIODevice *dev);

QImage loadGIFrame(QIODevice *dev, bool animation = false, const QImage &background = QImage(), int startX = 0, int startY = 0, int finishX = 0, int finishY = 0);
}

#endif
