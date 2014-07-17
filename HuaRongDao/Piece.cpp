#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <tuple>
#include <stdexcept>

#define STEP 1

class State;
std::unordered_set<State> g_State;

typedef enum PieceType{Zu = 1 , Jiang = 2 , Caocao = 4}PieceType;
typedef std::tuple<unsigned int , unsigned int> Location;
typedef enum Direction{Up = 1 , Down = 2 , Left = 3 , Right = 4}Direction;

class State//Mask
{

public:

    State(Location location , PieceType type);
    bool operator==(State const*);

    Location const& m_rLocation;
    PieceType const& m_rType;
    unsigned int getId();
    void setId(unsigned int);

private:

    Location m_Location;
    PieceType m_Type;

    unsigned int m_Id;
};

State::State(Location location , PieceType type):m_rLocation(m_Location) , m_rType(m_Type)
{
    m_Location = location;
    m_Type = type;
}

bool State::operator==(State const* rhs)
{
    return (m_Location == rhs->m_rLocation)&&(m_Type == rhs->m_rType)?true:false;
}

class CLPiece
{

public:

    CLPiece (unsigned int , unsigned int , Location , PieceType);
    bool moveUp(State*);
    bool moveDown(State*);
    bool moveLeft(State*);
    bool moveRight(State*);

    Location getLocation();

    int setLocation(Location location);

private:

    unsigned int m_Width;
    unsigned int m_Length;

    Location  m_Location;

    std::string m_Name;
    unsigned int m_Type;
};

CLPiece::CLPiece(unsigned int width , unsigned int length , Location location
                , PieceType type)
{
    m_Width    = width;
    m_Length   = lendth;
    m_Location = location;
    m_Type     = type;
}

bool CLPiece::moveUp(State* initial)
{
    unsigned int tmpX = 0;
    unsigned int tmpY= 0;

    std::tie(tmpX, tmpY) = getLocation();
    --tmpY;

    if(initial->getMask(std::make_tuple(tmpX , tmpY)))
    {

    }
}

#endif
