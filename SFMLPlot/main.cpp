#include <SFML/Graphics.hpp>
#include <math.h>
#include <limits>
#include <string>
#include <iostream>
#include <functional>

using namespace std;


const unsigned int INITIAL_WIDTH = 800;
const unsigned int INITIAL_HEIGHT = 600;
const float INITIAL_SCALE = 10;
const int INITIAL_POINT_COUNT = 1024;

const double EPSILON = 0.000001;

unsigned int windowWidth = INITIAL_WIDTH;
unsigned int windowHeight = INITIAL_HEIGHT;
float cameraX = 0;
float cameraY = 0;
float scale = INITIAL_SCALE;

double yFunction(double x)
{
    return sin(x) * x;
}

function<double(double)> createDerivativeFunction(function<double(double)> func)
{
    return [func](double x) -> double
    {
        // Standard formula for derivative
        return (func(x + EPSILON / 2) - func(x - EPSILON / 2)) / EPSILON;
    };
}

double findLocalExtremum(function<double(double)> func, double x)
{
    const double DIFF_EPSILON = EPSILON * sqrt(EPSILON);

    cout << endl << "Searching for extremum, start: " << x << endl;

    function<double(double)> derivative = createDerivativeFunction(func);
    function<double(double)> doubleDerivative = createDerivativeFunction(derivative);

    bool minimumSearch = false;

    if(abs(doubleDerivative(x)) < DIFF_EPSILON)
    {
        cout << "Can't find extremum at this point. (y''(x) == 0)" << endl;
        return numeric_limits<double>::quiet_NaN();
    }
    else if(doubleDerivative(x) > EPSILON)
    {
        cout << "y''(x) > 0, searching for minimum." << endl;
        minimumSearch = true;
    }
    else
    {
        cout << "y''(x) < 0, searching for maximum." << endl;
    }

    double diff = 0.;
    for(int steps = 0; steps < 10000000; steps++)
    {
        diff = EPSILON * derivative(x);
        if(isnan(diff))
        {
            cout << "No derivative at x = " << x << endl;
            return numeric_limits<double>::quiet_NaN();
        }
        x = minimumSearch ? (x - diff) : (x + diff);
//        cout << x << ' ' << diff << endl;
        if(abs(diff) < DIFF_EPSILON)
        {
            cout << "Steps: " << steps << endl;
            cout << "Extremum: " << x << endl;
            return x;
        }
    }
    cout << "Computation timed out. " << endl;
    return numeric_limits<double>::quiet_NaN();
}

sf::Vertex* plotVisibleChart(function<double(double)> func, int pointCount)
{
    // Create chart
    sf::Vertex* chart = new sf::Vertex[pointCount];
    for(int i = 0; i < pointCount; i++)
    {
        // Choose X coordinates that are visible on the screen
        double x = (((float)i + 0.5f - pointCount / 2) / (pointCount - 1) * windowWidth) / scale + cameraX;
        double y = func(x);

        if(isnan(y) || isinf(y))
        {
            chart[i].color = sf::Color::Transparent;
            chart[i].position = sf::Vector2f{x, 0};
        }
        else
        {
            chart[i].color = sf::Color::Magenta;
            chart[i].position = sf::Vector2f{x, y};
        }
    }
    return chart;
}

int main()
{
    int pointCount = INITIAL_POINT_COUNT;

    // Create the main window
    cout << "Hello!" << endl;
    sf::RenderWindow app{sf::VideoMode{windowWidth, windowHeight}, "Graph View"};

    // Create chart
    sf::Vertex* chart = plotVisibleChart(&yFunction, pointCount);

    // Extra graphics

    sf::Vertex lines[4];
    for(int i = 0; i < 4; i++)
    {
//        lines[i].color = sf::Color{200, 200, 200};
        lines[i].color = sf::Color::White;
    }

    sf::Vertex cameraLine[2];
    cameraLine[0].color = sf::Color{100, 100, 100};
    cameraLine[1].color = sf::Color{100, 100, 100};

    sf::Font font{};
    if(!font.loadFromFile("LiberationSans-Regular.ttf"))
    {
        app.close();
    }

    sf::Text zoomText{"", font, 24};
    zoomText.setFillColor(sf::Color::Cyan);

    sf::Text exitText{"Press M to find local extremum", font, 24};
    exitText.setFillColor(sf::Color::Cyan);
    exitText.setPosition(0, 30);

    // Start the display loop
    while(app.isOpen())
    {
        bool redraw = false;

        // Process events
        sf::Event event;
        while(app.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                app.close();
                return EXIT_SUCCESS;

            case sf::Event::KeyPressed:
                switch(event.key.code)
                {
                case sf::Keyboard::Up:
                    cameraY += 15 / scale;
                    redraw = true;
                    break;
                case sf::Keyboard::Down:
                    cameraY -= 15 / scale;
                    redraw = true;
                    break;
                case sf::Keyboard::Left:
                    cameraX -= 15 / scale;
                    redraw = true;
                    break;
                case sf::Keyboard::Right:
                    cameraX += 15 / scale;
                    redraw = true;
                    break;
                case sf::Keyboard::LBracket:
                    pointCount = max(8, pointCount / 2);
                    cout << "Points on screen: " << pointCount << endl << endl;
                    redraw = true;
                    break;
                case sf::Keyboard::RBracket:
                    pointCount *= 2;
                    cout << "Points on screen: " << pointCount << endl << endl;
                    redraw = true;
                    break;
                case sf::Keyboard::Dash:
                    scale = max(INITIAL_SCALE / 8192, scale * 0.5f);
                    redraw = true;
                    break;
                case sf::Keyboard::Equal:
                    scale = min(INITIAL_SCALE * 8192, scale * 2.f);
                    redraw = true;
                    break;
                case sf::Keyboard::Return:
                    app.close();
                    return EXIT_SUCCESS;
                case sf::Keyboard::M: {
                    double extremum = findLocalExtremum(&yFunction, cameraX);

                    if(!isnan(extremum))
                    {
                        for(int i = 0; i < pointCount; i++)
                        {
                            double x = chart[i].position.x;
                            if((extremum < x && x < cameraX) || (cameraX < x && x < extremum))
                            {
                                chart[i].color = sf::Color::White;
                            }
                        }
                    }
                    break;
                } default:
                    break;
                }
                break;
            case sf::Event::Resized:
                windowWidth = event.size.width;
                windowHeight = event.size.height;
                app.setView(sf::View{sf::FloatRect{0, 0, windowWidth, windowHeight}});
                redraw = true;
                break;
            default:
                break;
            }
        }

        if(redraw)
        {
            delete[] chart;
            chart = plotVisibleChart(yFunction, pointCount);
        }

        // Draw graphics
        app.clear();

        sf::Transform transform{};
        transform
        .translate(windowWidth / 2, windowHeight / 2)
        .scale(scale, -scale)
        .translate(-cameraX, -cameraY);
//            .scale(1, -1);


        lines[0].position = sf::Vector2f{0, cameraY - (float)windowWidth / scale};
        lines[1].position = sf::Vector2f{0, cameraY + (float)windowWidth / scale};
        lines[2].position = sf::Vector2f{cameraX - (float)windowHeight / scale, 0};
        lines[3].position = sf::Vector2f{cameraX + (float)windowHeight / scale, 0};

        cameraLine[0].position = sf::Vector2f{windowWidth / 2, 0};
        cameraLine[1].position = sf::Vector2f{windowWidth / 2, windowHeight};

        app.draw(cameraLine, 2, sf::Lines);
        app.draw(lines, 4, sf::Lines, transform);
        app.draw(chart, pointCount, sf::LineStrip, transform);

        zoomText.setString("Zoom: " + to_string(scale / INITIAL_SCALE));

        app.draw(zoomText);
        app.draw(exitText);

        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}
