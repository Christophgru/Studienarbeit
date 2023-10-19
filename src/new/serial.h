//
// Created by chris on 19.10.2023.
//

#ifndef STUDIENARBEIT_SERIAL_H
#define STUDIENARBEIT_SERIAL_H


class serial {
    ///Class to abstract the Serial Port access for Windows

private:
        int port;

public:
    /**Constructor
     * @param int port: port number to initialize*/
    serial(int port);


    /**initialize port
     * @returns int !=0 if error*/
    int initialize();

    /**get azimuth from bluetooth antenna
     * @returns float: winkel horizontal*/
    float getAngle();

};


#endif //STUDIENARBEIT_SERIAL_H
