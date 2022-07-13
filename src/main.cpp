#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "frame.h"
#include "sprite_chopper.h"

sf::Vector2f operator*(const sf::Vector2f &left, const sf::Vector2f &right)
{
    return sf::Vector2f(left.x * right.x, left.y * right.y);
}

void renderRect(sf::RenderWindow &window, const Frame &frame);

int main(int argc, char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(900, 900), "Frame_chopper");
    window.setFramerateLimit(30);

    std::string filename = "frames.coords";
    SpriteChopper sc(filename);
    sf::Texture texture;
    if (!texture.loadFromFile("sprite.png"))
    {
        // error...
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);

    sf::Vector2f mouse_position; 
    Frame current_rect; 
    bool is_select_area = false;
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape) window.close();             
                if (event.key.code == sf::Keyboard::S) sc.save();
                if (event.key.code == sf::Keyboard::L) sc.load();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                mouse_position = sf::Vector2f(position.x, position.y);
                current_rect.position_x = mouse_position.x;
                current_rect.position_y = mouse_position.y;
                is_select_area = true;
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                sf::Vector2f rect_size = sf::Vector2f(position.x, position.y) - mouse_position;
                if (rect_size.x < 0)
                {
                    current_rect.position_x += rect_size.x;
                    rect_size.x *= -1;
                }
                if (rect_size.y < 0)
                {
                    current_rect.position_y += rect_size.y;
                    rect_size.y *= -1;
                }
                current_rect.size_x = rect_size.x;
                current_rect.size_y = rect_size.y;
                sc.addNewFrame(Frame(
                            current_rect.position_x,
                            current_rect.position_y,
                            current_rect.size_x,
                            current_rect.size_y
                            ));
                is_select_area = false;
            } 
           
        }

        window.clear();
        window.draw(sprite);

        if (is_select_area == true)
        {
            sf::Vector2i position = sf::Mouse::getPosition(window);
            sf::Vector2f rect_size = sf::Vector2f(position.x, position.y) - mouse_position;
            current_rect.size_x = rect_size.x;
            current_rect.size_y = rect_size.y;
            renderRect(window, current_rect);
        }

        for (const auto & s : sc.getFrames())
        {
            renderRect(window, s);
        }

        window.display();
    }

    return 0;
}

void renderRect(sf::RenderWindow &window, const Frame &frame)
{
    sf::Vector2f position(frame.position_x, frame.position_y);
    sf::Vector2f size(frame.size_x, frame.size_y);

    sf::RectangleShape line;
    line.setFillColor(sf::Color::Red);

    //up
    line.setPosition(position);
    line.setSize(size * sf::Vector2f(1, 0) + sf::Vector2f(5, 5));
    window.draw(line);

    //left
    line.setPosition(position);
    line.setSize(size * sf::Vector2f(0, 1) + sf::Vector2f(5, 5));
    window.draw(line);

    //down
    line.setPosition(position + size * sf::Vector2f(0, 1));
    line.setSize(size * sf::Vector2f(1, 0) + sf::Vector2f(5, 5));
    window.draw(line);

    //right
    line.setPosition(position + size * sf::Vector2f(1, 0));
    line.setSize(size * sf::Vector2f(0, 1) + sf::Vector2f(5, 5));
    window.draw(line);
}




