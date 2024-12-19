#include "../../includes/TEng/Menu.h"

void UI_Button::setType(int type, SDL_Renderer* _renderer) {
    renderer = _renderer;
    if(type == 0) {
        w = width;
        h = height;

        texture.setup(w,h,renderer);
        texture.loadFromFile("button.png");

        hoverTexture.setup(w,h,renderer);
        hoverTexture.loadFromFile("button1.png");
    } else if(type == 1) {
        w = smallWidth;
        h = smallHeight;

        texture.setup(w,h,renderer);
        texture.loadFromFile("upgrade-menu/small.png");

        hoverTexture.setup(w,h,renderer);
        hoverTexture.loadFromFile("upgrade-menu/small-selected.png");

        activeTexture.setup(w,h,renderer);
        activeTexture.loadFromFile("upgrade-menu/small-active.png");
    } else if(type == 2) {
        w = weaponWidth;
        h = weaponWidth;

        texture.setup(w,h,renderer);
        texture.loadFromFile("upgrade-menu/upgrade-button.png");

        hoverTexture.setup(w,h,renderer);
        hoverTexture.loadFromFile("upgrade-menu/upgrade-selected.png");

        activeTexture.setup(w,h,renderer);
        activeTexture.loadFromFile("upgrade-menu/upgrade-active.png");
    } else if(type == 3) {
        w = scaleUI(60);
        h = scaleUI(30);

        texture.setup(w,h,renderer);
        texture.loadFromFile("upgrade-menu/weapon.png");

        hoverTexture.setup(w,h,renderer);
        hoverTexture.loadFromFile("upgrade-menu/weapon-selected.png");

        activeTexture.setup(w,h,renderer);
        activeTexture.loadFromFile("upgrade-menu/weapon-active.png");
    }
}



UI_Button::UI_Button(const int x, const int y, const std::string& text, SDL_Renderer* renderer, Sound* sound, const SDL_Color * color, TTF_Font* font, void (*action)(GlobalGameState& state, int attr1, int attr2),GlobalGameState& state, const int type, int attribute, int attribute2) :
x(x), y(y), type(type), buttonClick(sound), action(action), state(state), attribute(attribute), attribute2(attribute2) {
    setType(type, renderer);
    usingText = true;

    textTexture.setup(0,0,renderer);
    textTexture.loadFromRenderedText(text, *color, font);
}

UI_Button::UI_Button(const int x, const int y, const std::string& path, SDL_Renderer* renderer, Sound* sound, void (*action)(GlobalGameState& state, int attr1, int attr2),GlobalGameState& state, const int type, int attribute, int attribute2, const std::string& secondaryPath) : x
(x), y(y), type(type), buttonClick(sound), action(action), state(state), attribute(attribute), attribute2(attribute2) {
    setType(type, renderer);

    usingText = false;

    imageTexture.setup(w,h,renderer);
    imageTexture.loadFromFile(path);

    if(secondaryPath != "no") {
        secondaryImage = true;
        secondImageTexture.setup(w,h,renderer);
        secondImageTexture.loadFromFile(secondaryPath);
    }

}

bool UI_Button::mouseEvent(const int mouseX, const int mouseY) const {
    if(mouseX >= x && mouseX <= x+w && mouseY >= y && mouseY <= y+h) {
        return true;
    }
    return false;
}

void UI_Button::render() const {
    texture.render(x,y);
    if(active) {
        activeTexture.render(x,y);
    }
    if(selected) {
        hoverTexture.render(x,y);
    }
    if(usingText) {
        textTexture.render(x+(w-textTexture.getWidth())/2,y+(h-textTexture.getHeight())/2);
    } else {
        if(disabled) {
            SDL_SetTextureColorMod(imageTexture.getTexture(),60,60,60);
        } else {
            SDL_SetTextureColorMod(imageTexture.getTexture(),255,255,255);
        }
        imageTexture.render(x,y);
        if(secondaryImage) {
            if(disabled) {
                SDL_SetTextureColorMod(secondImageTexture.getTexture(),60,60,60);
            } else {
                SDL_SetTextureColorMod(secondImageTexture.getTexture(),255,255,255);
            }
            secondImageTexture.render(x,y);
        }
    }
}

UI_Button* UI_Button::move(const MENU_CONTROL action) const {
    switch(action) {
    case MENU_CONTROL::up: {
        if (buttonAbove != nullptr && !buttonAbove->isDisabled()) {
            return buttonAbove;
        }
        return nullptr;
    }
    case MENU_CONTROL::down: {
        if (buttonBelow != nullptr && !buttonBelow->isDisabled()) {
            return buttonBelow;
        }
        return nullptr;
    }
    case MENU_CONTROL::left: {
        if (buttonLeft != nullptr && !buttonLeft->isDisabled()) {
            return buttonLeft;
        }
        return nullptr;
    }
    case MENU_CONTROL::right: {
        if (buttonRight != nullptr && !buttonRight->isDisabled()) {
            return buttonRight;
        }
        return nullptr;
    }
    default:
        return nullptr;
    }
}

void UI_Button::link(const RELATIVE_DIRECTION direction, UI_Button* button) {
    switch(direction) {
        case RELATIVE_DIRECTION::above:
            buttonAbove = button;
            break;
        case RELATIVE_DIRECTION::below:
            buttonBelow = button;
            break;
        case RELATIVE_DIRECTION::left:
            buttonLeft = button;
            break;
        case RELATIVE_DIRECTION::right:
            buttonRight = button;
            break;
    }
}

int UI_Menu::addButton(int x, int y, const std::string &text, SDL_Color* color, TTF_Font* font,
    const int above, const int below, const int left, const int right, void (*action)(GlobalGameState& state, int attr1, int attr2),GlobalGameState& state, int type, int attribute, int attribute2) {
        buttons.emplace_back(x, y, text, renderer, buttonSound, color, font, action,state, type, attribute, attribute2);
        if(above != -1) {
            buttons.back().link(RELATIVE_DIRECTION::above, &buttons[above]);
            buttons[above].link(RELATIVE_DIRECTION::below, &buttons.back());
        }
        if(below != -1) {
            buttons.back().link(RELATIVE_DIRECTION::below, &buttons[below]);
            buttons[below].link(RELATIVE_DIRECTION::above, &buttons.back());
        }
        if(left != -1) {
            buttons.back().link(RELATIVE_DIRECTION::left, &buttons[left]);
            buttons[left].link(RELATIVE_DIRECTION::right, &buttons.back());
        }
        if(right != -1) {
            buttons.back().link(RELATIVE_DIRECTION::right, &buttons[right]);
            buttons[right].link(RELATIVE_DIRECTION::left, &buttons.back());
        }
        return static_cast<int>(buttons.size())-1;
    }

int UI_Menu::addButton(int x, int y, const std::string &path, const int above, const int below, const int left, const int right, void (*action)(GlobalGameState& state, int attr1, int attr2),GlobalGameState& state, int type, int attribute, int attribute2, const std::string& secondaryPath) {
    buttons.emplace_back(x, y, path, renderer,buttonSound,action,state, type, attribute, attribute2, secondaryPath);
    if(above != -1) {
        buttons.back().link(RELATIVE_DIRECTION::above, &buttons[above]);
        buttons[above].link(RELATIVE_DIRECTION::below, &buttons.back());
    }
    if(below != -1) {
        buttons.back().link(RELATIVE_DIRECTION::below, &buttons[below]);
        buttons[below].link(RELATIVE_DIRECTION::above, &buttons.back());
    }
    if(left != -1) {
        buttons.back().link(RELATIVE_DIRECTION::left, &buttons[left]);
        buttons[left].link(RELATIVE_DIRECTION::right, &buttons.back());
    }
    if(right != -1) {
        buttons.back().link(RELATIVE_DIRECTION::right, &buttons[right]);
        buttons[right].link(RELATIVE_DIRECTION::left, &buttons.back());
    }
    return static_cast<int>(buttons.size())-1;
}

void UI_Menu::render() const {
    titleTexture.render(titleX,titleY);
    for(auto& button : buttons) {
        button.render();
    }
    for(auto& button : buttons) {
        if(button.hasHover() && button.isSelected()) {
            button.renderHover(button.getX()+button.getWidth(),button.getY());
        }
    }
}

void UI_Menu::loadMenu() {
    for(auto& button : buttons) {
        button.deselect();
    }
    buttons[0].select();
    currentButton = &buttons[0];
}

void UI_Button::click() {
    action(state, attribute, attribute2);
    buttonClick->play();
}



void UI_HoverText::addLine(const std::string &lineText, const std::string& lineValue, TTF_Font *font, SDL_Color color) {
    text.emplace_back();
    text.back().setup(renderer);
    text.back().loadFromRenderedText(lineText,color,font);
    values.emplace_back();
    values.back().setup(renderer);
    values.back().loadFromRenderedText(lineValue,color,font);
}

void UI_HoverText::render(const int x, const int y) const {

    int widestWidth = 0;
    int breakPoint = 0;
    for(int i = 0; i < text.size(); i++) {
        if(widestWidth < text[i].getWidth()+2+values[i].getWidth()+30) {
            widestWidth = text[i].getWidth()+2+values[i].getWidth()+30;
            breakPoint = 10+text[i].getWidth();
        }
    }

    const SDL_Rect rect = {x,y,widestWidth,scaleUI(15*text.size())};
    SDL_SetRenderDrawColor(renderer, 26, 26, 26, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderDrawRect(renderer,&rect);

    for(int i = 0; i < text.size(); i++) {
        text[i].render(x+breakPoint-text[i].getWidth(),y+scaleUI(15*i));
        values[i].render(x+breakPoint+2,y+scaleUI(15*i));
    }
}



