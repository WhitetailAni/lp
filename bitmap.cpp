// C++
#include <iostream>
#include <iomanip>
#include <vector>
 
// Qt
#include <QCoreApplication>
#include <QImage>
#include <QImageReader>
 
using namespace std;
 
class ESCPOSImage {
private:
    int m_width;
    int m_height;
    int m_gs_x;
    int m_gs_y;
    int m_gs_k;
    // Actual bytes for image - could have used QByteArray for this, I guess.
    vector <unsigned char> m_bytes;
 
    // This turns on a pixel a position x, y as you would expect. 
    // The bytes in the bitmap is stored in a weird "y first", "x second" order.
    // The bit fiddling here takes care of it.
    void setPixel( int x, int y ) {
        size_t byte_index = x * m_gs_y + y / 8;
        int bit_no = y % 8;
        // Swap msb/lsb order. This probably only works on machines with "normal" endianess.....
        unsigned char bit = 1 << ( 7 - bit_no );
        m_bytes.at( byte_index ) = m_bytes.at( byte_index ) | bit;
    }
 
public:
    ESCPOSImage( const QImage & image ) {
        // Set up x and y as pr. epson manual
        m_width = image.width();
        m_height = image.height();
 
        m_gs_x = m_width / 8 + (m_width % 8 != 0?1:0);
        m_gs_y = m_height / 8 + (m_height % 8 != 0?1:0);
         
         if ( m_gs_x > 255 || m_gs_y > 255 ) {
            // You may want to write an error message here
            throw "Too large on dimension"; 
        }
 
        m_gs_k = m_gs_x * m_gs_y * 8;
        // Bit unsure about this limit. It depends on the actual printer....
        if ( m_gs_k > (3072*8) ) {
            // You may want to write an error message here
            throw "Too large on area"; 
        }
 
        vector<unsigned char> bytes( m_gs_k, 0 ); // Blank all bytes.
        m_bytes = bytes;
         
        // Iterate over the image, turn on any pixels that are set in the monochromo image.
        for ( int i_y = 0; i_y < m_height; ++i_y ) {
            for ( int i_x = 0; i_x < m_width; ++i_x ) {
                if ( image.pixelIndex( i_x, i_y ) == Qt::color1 ) {
                    setPixel( i_x, i_y );
                }
            }
        }
    }
 
    // Access internal representation. Should be const something, I guess.
    vector<unsigned char> & getBytes() {
        return m_bytes;
    }
 
    // Bytes suitable to send to the printer to define the bitmap.
    QByteArray getGSStar() {
        QByteArray res( m_bytes.size() + 4, 0 );
        res[0] = 29;
        res[1] = '*';
        res[2] = (unsigned char) m_gs_x;
        res[3] = (unsigned char) m_gs_y;
        for ( size_t i = 0; i < m_bytes.size(); ++i ) {
            res[ int(4 + i) ] = m_bytes.at( i );
        }
        return res;
    };
 
};
 
int main( int argc, char ** argv ) {
    // Note, that exception code is not handled here!
    // If you get weird errors, try putting in an exception handler around the body of main.
 
    // Fire up Qt.
    QCoreApplication app( argc, argv );
  
    // Assume the user was friendly enough to pass the name of an image, load it.
QImageReader imageReader( argv[1] );
    QImage orgImage = imageReader.read();
    if ( orgImage.isNull() ) {
        cerr << "Unable to load image - QImageReader error: " << imageReader.errorString().toStdString() << endl;
        return 1;
    }
 
    // Convert to monochrome. The conversion done by Qt is a bit bad. So probably a good idea to be monochrome already...
    QImage monoImage = orgImage.convertToFormat( QImage::Format_Mono, Qt::MonoOnly );
    if ( monoImage.isNull() ) {
        cerr << "Unable to convert image to monochrome" << endl;
        return 1;
    }
 
    // Create the ESCPOS image representation, get the bytes that represents the GS Star command, and dump the raw bytes to stdout.
    ESCPOSImage posImage( monoImage );
    QByteArray ar = posImage.getGSStar();
    for ( int i = 0; i < ar.size(); ++i ) {
        cout << (unsigned char) ar[i];
    }
    cout << "\n"; // This may not be needed, actually.
 
    // Do a GS / to actually output the bitmap.
    cout << (unsigned char) 29 << "/" << (unsigned char) 0 << "\n";
     
    return 0;
}
