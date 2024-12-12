#ifndef UVREGION_H
#define UVREGION_H

namespace Essentia
{
    struct UVRegion 
    {
        float uMin, vMin; // Esquina inferior izquierda
        float uMax, vMax; // Esquina superior derecha
    };
}

#endif // !UVREGION_H