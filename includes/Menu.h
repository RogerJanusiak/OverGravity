
#ifndef MENU_H
#define MENU_H
#include "GlobalConstants.h"
#include "Sound.h"
#include "State.h"
#include "Texture.h"

#include <SDL_render.h>
#include <vector>

enum class MENU_CONTROL {
  connect,
  disconnect,
  up,
  down,
  select,
  back,
  left,
  right,
};

enum class RELATIVE_DIRECTION {
  above,
  below,
  left,
  right,
};

class UI_HoverText {

public:

  UI_HoverText() = default;

  void setup(int numberOfLines, SDL_Renderer* _renderer) { text.reserve(numberOfLines);values.reserve(numberOfLines);renderer = _renderer; }
  void addLine(const std::string &lineText, const std::string& lineValue, TTF_Font *font, SDL_Color color);

  void render(int x, int y) const;

private:

  std::vector<Texture> text;
  std::vector<Texture> values;
  SDL_Renderer* renderer = nullptr;

};

class UI_Button {

public:

  UI_Button(int x, int y, const std::string& text, SDL_Renderer* renderer,
            const SDL_Color * color, TTF_Font* font, void (*action)(State& state, int attr1, int attr2),State& state, int type = 0, int attribute = 0,int attribute2 = 0);
  UI_Button(int x, int y, const std::string& path, SDL_Renderer* renderer, void (*action)(State& state, int attr1, int attr2),State& state, int type = 0, int attribute = 0,int attribute2 = 0, const std::string& secondaryPath = "no");

  void render() const;
  [[nodiscard]] int getWidth() const { return w; }

  [[nodiscard]] UI_Button* move(MENU_CONTROL action) const;
  void linkButtons(UI_Button* above, UI_Button* below,UI_Button* left, UI_Button* right) { buttonAbove = above; buttonBelow = below; buttonLeft = left; buttonRight = right; }
  void link(RELATIVE_DIRECTION direction, UI_Button* button);

  void select() {selected = true;}
  void deselect() {selected = false;}

  void activate() { active = true; }
  void deactivate() { active = false; }
  [[nodiscard]] bool isActive() const { return active; }

  void setupHover(int numberOflines) { ht.setup(numberOflines,renderer);hover = true; }
  [[nodiscard]] bool hasHover() const { return hover; }
  void addLine(const std::string &lineText, const std::string& lineValue, TTF_Font *font, SDL_Color color) { ht.addLine(lineText,lineValue,font,color); }
  void renderHover(int x, int y) const { ht.render(x,y); }

  void disable() { disabled = true; }
  void enable() { disabled = false; }
  [[nodiscard]] bool isDisabled() const { return disabled; }

  [[nodiscard]] bool isSelected() const {return selected;}

  void setType(int type , SDL_Renderer* _renderer);

  void click();

  int getType() const { return type; }
  int getX() const { return x; }
  int getY() const { return y; }

  [[nodiscard]] bool mouseEvent(int mouseX, int mouseY) const;

  static constexpr int width = 160*1.5*SCALE_FACTOR;
  static constexpr int height = 32*1.5*SCALE_FACTOR;

  static constexpr int smallWidth = 160*0.8*SCALE_FACTOR;
  static constexpr int smallHeight = 32*0.8*SCALE_FACTOR;

  static constexpr int weaponWidth = 60*SCALE_FACTOR;

private:

  int x, y;
  int w = 0, h = 0;

  int attribute;
  int attribute2;

  int type;
  bool usingText;

  UI_HoverText ht;
  bool hover = false;

  State& state;
  SDL_Renderer* renderer;
  void (*action)(State& state, int attr1, int attr2);

  UI_Button* buttonAbove = nullptr;
  UI_Button* buttonBelow = nullptr;
  UI_Button* buttonRight = nullptr;
  UI_Button* buttonLeft = nullptr;

  Texture texture;
  Texture hoverTexture;
  Texture imageTexture;
  Texture secondImageTexture;
  Texture textTexture;
  Texture activeTexture;

  Sound buttonClick;

  bool selected = false;
  bool secondaryImage = false;

  bool disabled = false;
  bool active = false;

};

class UI_Menu {
public:

  explicit UI_Menu(const int numberOfButtons) { buttons.reserve(numberOfButtons); };

  int addButton(int x, int y, const std::string& text, SDL_Color* color, TTF_Font* font,
    int above, int below, int left, int right, void (*action)(State& state, int attr1, int attr2),State& state, int type = 0, int attribute = 0,int attribute2 = 0);
  int addButton(int x, int y, const std::string& path, int above, int below, int left, int right, void (*action)(State& state, int attr1, int attr2),State& state, int type = 0, int attribute = 0,int attribute2 = 0, const std::string& secondaryPath = "no");
  std::vector<UI_Button>* getButtons() {return &buttons;}

  void reset() { buttons.clear(); };

  [[nodiscard]] UI_Button* loadMenu();
  void addRenderer(SDL_Renderer* _renderer) { renderer = _renderer; }

  void addTitle(const int x, const int y, const Texture & texture) { titleX = x, titleY = y, titleTexture = texture; }

  void render() const;

private:

  TTF_Font* font = nullptr;
  SDL_Color* color = nullptr;

  SDL_Renderer* renderer = nullptr;
  Texture titleTexture;
  int titleX = 0,titleY = 0;
  std::vector<UI_Button> buttons;

};

#endif //MENU_H