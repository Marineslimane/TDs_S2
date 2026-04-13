#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream> // to read files
#include <unordered_set>
#include <unordered_map>

// EXERCISE 1 :

size_t folding_string_hash(std::string const& s, size_t max) // simple hashing
{
    size_t hash {0};

    for (size_t i {0}; i < s.size() ; i++)
    {
        hash += s[i];
        hash %= max; // doing it each time in the loop to avoid hash getting too big
    }

    return hash;
}

size_t folding_string_ordered_hash(std::string const& s, size_t max)
{
    size_t hash {0};

    for (size_t i {0}; i < s.size() ; i++)
    {
        hash += s[i]*(i+1);
        hash %= max; // doing it each time in the loop to avoid hash getting too big
    }

    return hash;
}

// EXERCISE 2 :
// Q1 :
enum Direction {Haut, Droite, Bas, Gauche};

struct Position 
{
    int x;
    int y;
};

bool operator==(Position const& a, Position const& b) 
{
    return (a.x == b.x && a.y == b.y);
}

std::ostream& operator<<(std::ostream& os, Position const& a) 
{
    os << "(" << a.x << "," << a.y << ")";
    return os;
}

Position operator+(Position const& a, Position const& b) 
{
    return {a.x + b.x, a.y + b.y};
}

Position operator+=(Position& a, Position const& b) 
{
    a = a + b;
    return a;
}

Position operator+(Position const& a, Direction const& d) 
{
    Position p {a};
    switch(d)
    {
        case Haut:
            p.y -= 1; 
            break;
        case Droite:
            p.x += 1;
            break;
        case Bas:
            p.y += 1; 
            break;
        case Gauche:
            p.x -= 1; 
            break;
    }

    return p;
}

Position operator+=(Position& a, Direction const& d) 
{
    a = a + d;
    return a;
}

bool operator!=(Position const& a, Position const& b) // added by me
{
    return (a.x != b.x || a.y != b.y);
}

Direction turn_right(Direction const& d)
{
    Direction new_d;

    switch(d)
    {
        case Haut:
            new_d = Droite;
            break;
        case Droite:
            new_d = Bas;
            break;
        case Bas:
            new_d = Gauche;
            break;
        case Gauche:
            new_d = Haut;
            break;
    }

    return new_d;
}

// Q2 :
struct Input_Structure
{
    Position p_init; // initial position of the guard
    Direction d_init; // initial direction of the guard

    std::vector<Position> obstacles; // positions of "#" symbols on the map

};

Input_Structure read_input(std::istream& input_stream)
{
    Input_Structure map {};
    int j {0}; // index of line ie j represents y value

    for (std::string line{}; std::getline(input_stream, line, '\n') && line != "";) // line by line
    {
        for (int i {0} ; i < line.size() ; i++) // i represents x value
        {
            switch(line[i])
            {
                case '^':
                    map.d_init = Haut;
                    map.p_init = {i,j};
                    break;
                case '>':
                    map.d_init = Droite;
                    map.p_init = {i,j};
                    break;
                case 'v':
                    map.d_init = Bas;
                    map.p_init = {i,j};
                    break;
                case '<':
                    map.d_init = Gauche;
                    map.p_init = {i,j};
                    break;
                case '#':
                    map.obstacles.push_back({i,j});
                    break;
                default:
                    break;
            }
        }

        j++;
    }

    return map;
}

// Q3 :

// 1.
namespace std 
{
    template <>

    struct hash<Position> // hash function for struct Position 
    {
        std::size_t operator()(const Position& pos) const 
        {
            return std::hash<int>()(pos.x)^std::hash<int>()(pos.y); // folding hash : combines hashes of x and y coordinates with XOR operator
        }
    };
}

struct WalkResult // to represent informations at the end of the simulation
{
    Position final_position;
    size_t steps_taken;
    std::unordered_set<Position> visited_positions;
};

// 2.
WalkResult simulate_guard(Input_Structure s, const int h, const int w)
{
    Position p_guard {s.p_init};
    Direction d_guard {s.d_init};

    WalkResult result {};

    bool is_out {false};
    
    while (!is_out)
    {
        Position new_p = p_guard + d_guard;

        auto is_obstacle {std::find(s.obstacles.begin(), s.obstacles.end(), new_p)}; // checking if the new position is an obstacle

        if (is_obstacle != s.obstacles.end()) // if the new position is an obstacle
        {
            d_guard = turn_right(d_guard); // guard must turn right when encountering an obstacle
        }

        // other way - manual research in the vector of obstacles : 
       /*  for (Position obstacle : s.obstacles) // for each position that is an obstacle
        {
            if (new_p == obstacle)
            {
                d_guard = turn_right(d_guard); // guard must turn right when encountering an obstacle
                break;
            }
        } */

        // updating the position of the guard and result :
        p_guard += d_guard;
        result.steps_taken++;
        result.visited_positions.insert(p_guard); // by default, repetitions are ignored by unordered_set

        // checking if the guard is out of the map : 
        is_out = ((p_guard.y == h-1 && d_guard == Bas) || (p_guard.y == 0 && d_guard == Haut) || (p_guard.x == w-1 && d_guard == Droite) || (p_guard.x == 0 && d_guard == Gauche));
    }

    result.final_position = p_guard;
    return result;
}

// Q4 : 
struct GuardState
{
    Position p;
    Direction d;
};

bool operator==(GuardState const& s1, GuardState const& s2)
{
    return (s1.p == s2.p && s1.d == s2.d);
}

// adding the hash operator for struct GuardState, just like for struct Position : 
namespace std 
{
    template <>

    struct hash<GuardState> // hash function for struct Position 
    {
        std::size_t operator()(const GuardState s) const 
        {
            return std::hash<int>()(s.p.x)^std::hash<int>()(s.p.y)^std::hash<int>()(static_cast<int>(s.d)); // folding hash : combines hashes of x and y coordinates with XOR operator and for enum Direction, static cast to int
        }
    };
}

bool is_loop(Input_Structure s, const int h, const int w)
{
    Position p_guard {s.p_init};
    Direction d_guard {s.d_init};
    GuardState current_state {p_guard, d_guard};

    std::unordered_set<GuardState> guard_states {};

    bool is_out {false};

    while (!is_out)
    {
        auto is_visited {std::find(guard_states.begin(), guard_states.end(), current_state)};

        if (is_visited != guard_states.end()) // if state already visited
        {
            return true; // it's a loop
        }

        guard_states.insert(current_state);

        Position new_p = p_guard + d_guard;

        auto is_obstacle {std::find(s.obstacles.begin(), s.obstacles.end(), new_p)};

        if (is_obstacle != s.obstacles.end()) // if the new position is an obstacle
        {
            d_guard = turn_right(d_guard); // guard must turn right when encountering an obstacle
        }

        p_guard += d_guard;
        current_state = {p_guard, d_guard};

        // checking if the guard is out of the map :
        is_out = ((p_guard.y == h-1 && d_guard == Bas) || (p_guard.y == 0 && d_guard == Haut) || (p_guard.x == w-1 && d_guard == Droite) || (p_guard.x == 0 && d_guard == Gauche));
    }

    return false; // guard exited normally, no loop
}

int count_loops(Input_Structure s, const int h, const int w)
{
    WalkResult r {simulate_guard(s, h, w)};

    int count {0};

    for (Position p : r.visited_positions) // for all guard positions
    {
        s.obstacles.push_back(p); // temporarely becomes an obstacle

        if (is_loop(s, h, w)) 
        {
            count++;
        }

        s.obstacles.pop_back();
    }

    return count;
}

// Exercise 3 :
enum class CardKind 
{
    Heart,
    Diamond,
    Club,
    Spade,
};

enum class CardValue 
{
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Ace,
};

struct Card 
{
    CardKind kind;
    CardValue value;

    size_t hash() const // hashing method for struct Card
    {
        return static_cast<int>(kind)^static_cast<int>(value);
    }

};

bool operator==(Card const& c1, Card const& c2)
{
    return (c1.kind == c2.kind && c1.value == c2.value);
}

namespace std 
{
    template<>

    struct hash<Card> 
    {
        size_t operator()(Card const& card) const 
        {
            return card.hash();
        }
    };
}

std::vector<Card> get_cards(size_t const size) // generates a vector of random cards
{
    std::vector<Card> cards {};
    cards.reserve(size);

    for (size_t i {0}; i < size; ++i) 
    {
        cards.push_back({static_cast<CardKind>(rand() % 4), static_cast<CardValue>(rand() % 13)});
    }

    return cards;
}

std::unordered_map<Card, int> count_cards()
{
    std::vector<Card> cards {get_cards(100)}; // generates 100 random cards
    std::unordered_map<Card, int> result {};

    for (Card card : cards)
    {
        auto is_in {result.find(card)}; // checking if the card is already a key of the unordered_map

        if (is_in == result.end()) // if not
        {
            result[card] = 1;
        }
        else // if it is
        {
            result[card]++;
        }
    }

    return result;
}

std::string card_name(Card const& card) 
{
    std::string name {};

    unsigned int card_value {(static_cast<unsigned int>(card.value)+2) % 14};

    if (card_value < 10) {
        name += '0' + std::to_string(card_value);
    }else if (card_value == 10) {
        name += "10";
    }else if (card_value == 11) {
        name += 'J';
    }else if (card_value == 12) {
        name += 'Q';
    }else if (card_value == 13) {
        name += 'K';
    }

    name += " of ";

    if (card.kind == CardKind::Heart) {
        name += "Heart";
    }else if (card.kind == CardKind::Diamond) {
        name += "Diamond";
    }else if (card.kind == CardKind::Club) {
        name += "Club";
    }else if (card.kind == CardKind::Spade) {
        name += "Spade";
    }
    return name;
}

int main()
{
    // TESTS :

    // exercise 1 : 
    std::string two_letters {"ab"};
    std::string two_letters_reversed {"ba"};

    {
        std::cout << "SIMPLE HASHING :" << std::endl;
        std::cout << "Two letters word result : "  << folding_string_hash(two_letters, 100) << std::endl;
        std::cout << "Reversed two letters word result : "  << folding_string_hash(two_letters_reversed, 100) << std::endl;
        std::cout << "" << std::endl;
    }

    {
        std::cout << "ORDERED HASHING :" << std::endl;
        std::cout << "Two letters word result : "  << folding_string_ordered_hash(two_letters, 100) << std::endl;
        std::cout << "Reversed two letters word result : "  << folding_string_ordered_hash(two_letters_reversed, 100) << std::endl;
    }

    // exercise 2 : 
    Position A {0,0};
    Position B {0,1};
    std::cout << (A == B) << std::endl;
    std::cout << A << std::endl;
    std::cout << (A + B) << std::endl;
    // std::cout << (A += B) << std::endl;
    std::cout << (A + Haut) << std::endl;
    std::cout << (A += Haut) << std::endl;
    Position pos {(2, 3)};
    Direction dir = Direction::Haut;
    Position newPos { pos + dir }; // newPos devient (2, 2)
    newPos += dir; // newPos devient (2, 1)

    std::ifstream file(R"(C:\Users\marin\Documents\IMAC\TDs_S2\TD5\inputs\small_map.txt)");
    Input_Structure s {read_input(file)};
    WalkResult r {simulate_guard(s, 10, 10)};
    std::cout << "Number of visited positions :  ";
    std::cout << r.visited_positions.size() << std::endl;

    for (Position p : r.visited_positions)
    {
        std::cout << p << std::endl;
    }

    std::cout << "All positions of obstacles in carte.txt : " << std::endl;

    for (Position p : s.obstacles)
    {
        std::cout << p << std::endl;
    }

    std::ifstream file2(R"(C:\Users\marin\Documents\IMAC\TDs_S2\TD5\inputs\input_guard_patrol.txt)");
    Input_Structure s2 {read_input(file2)};
    WalkResult r2 {simulate_guard(s2, 130, 130)};
    std::cout << "Number of visited positions :  ";
    std::cout << r2.visited_positions.size() << std::endl;

    // std::cout <<  "Number of loops :  " << count_loops(s2, 130, 130) << std::endl;

    // exercise 3 : 
    std::unordered_map<Card, int> cards_map {count_cards()};

    for (auto const& duo : cards_map)
    {
        std::cout <<  "Number of " << card_name(duo.first) << " cards in deck :  " << duo.second << std::endl;
    }

    return 0;
}