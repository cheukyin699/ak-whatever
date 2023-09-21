#include <iostream>
#include <memory>
#include <stdexcept>

#include <spine-sfml.h>
#include <SFML/Graphics.hpp>

std::shared_ptr<spine::SkeletonData> readSkeletonBinaryData(const char* filename, spine::Atlas* atlas, float scale) {
    spine::SkeletonBinary binary(atlas);
    binary.setScale(scale);

    auto skeletonData = binary.readSkeletonDataFile(filename);
    if (!skeletonData) {
        throw std::runtime_error(binary.getError().buffer());
    }

    return std::shared_ptr<spine::SkeletonData>(skeletonData);
}

void callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* event) {
}

int main() {
    spine::SFMLTextureLoader textureLoader;
    auto atlas = std::make_unique<spine::Atlas>("data/ling/char_2023_ling.atlas", &textureLoader);
    auto skellyData = readSkeletonBinaryData("data/ling/char_2023_ling.skel", atlas.get(), 0.5f);

    // Configure mixing
    spine::AnimationStateData stateData(skellyData.get());

    std::cout << "All animations:" << '\n';
    size_t animationIndex = 0;
    auto animations = skellyData->getAnimations();
    for (size_t i = 0; i < animations.size(); i++) {
        std::cout << '-' << animations[i]->getName().buffer() << '\n';
    }

    spine::SkeletonDrawable drawable(skellyData.get(), &stateData);
    drawable.timeScale = 1;
    drawable.setUsePremultipliedAlpha(true);
    drawable.state->setListener(callback);
    drawable.state->addAnimation(0, "Idle", true, 0);

    auto skeleton = drawable.skeleton;
    skeleton->setToSetupPose();
    skeleton->setPosition(320, 590);
    skeleton->updateWorldTransform();

    spine::SkeletonBounds bounds;

    sf::RenderWindow w(sf::VideoMode(640, 640), "Arknights", sf::Style::Close);
    w.setFramerateLimit(60);
    sf::Event evt;
    sf::Clock deltaClock;

    while (w.isOpen()) {
        while (w.pollEvent(evt)) {
            switch (evt.type) {
                case sf::Event::Closed:
                    w.close();
                    break;
                case sf::Event::MouseButtonReleased:
                    {
                        animationIndex = (animationIndex + 1) % animations.size();
                        auto animationName = animations[animationIndex]->getName().buffer();
                        std::cout << "Currently playing animation: " << animationName << std::endl;
                        drawable.state->clearTracks();
                        drawable.state->addAnimation(0, animationName, true, 0);
                        break;
                    }
                default:
                    break;
            }
        }

        float delta = deltaClock.getElapsedTime().asSeconds();
        deltaClock.restart();

        bounds.update(*skeleton, true);

        drawable.update(delta);

        w.clear();
        w.draw(drawable);
        w.display();
    }

    return 0;
}
