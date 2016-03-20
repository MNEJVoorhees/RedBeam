#ifndef TRACER_H
#define TRACER_H

#include <unordered_map>
#include <vector>
#include "hw/holly/tile_renderer.h"
#include "hw/holly/trace.h"
#include "ui/window_listener.h"

namespace re {

namespace ui {
class Window;
}

namespace emu {

struct TextureInst {
  hw::holly::TSP tsp;
  hw::holly::TCW tcw;
  const uint8_t *palette;
  const uint8_t *texture;
  renderer::TextureHandle handle;
};

typedef std::unordered_map<hw::holly::TextureKey, TextureInst> TextureMap;

class TraceTextureCache : public hw::holly::TextureProvider {
 public:
  const TextureMap::iterator textures_begin() { return textures_.begin(); }
  const TextureMap::iterator textures_end() { return textures_.end(); }

  void AddTexture(const hw::holly::TSP &tsp, hw::holly::TCW &tcw,
                  const uint8_t *palette, const uint8_t *texture);
  void RemoveTexture(const hw::holly::TSP &tsp, hw::holly::TCW &tcw);
  renderer::TextureHandle GetTexture(
      const hw::holly::TSP &tsp, const hw::holly::TCW &tcw,
      hw::holly::RegisterTextureCallback register_cb);
  renderer::TextureHandle GetTexture(hw::holly::TextureKey texture_key);

 private:
  TextureMap textures_;
};

class Tracer : public ui::WindowListener {
 public:
  Tracer(ui::Window &window);
  ~Tracer();

  void Run(const char *path);

 private:
  void OnPaint(bool show_main_menu) final;
  void OnKeyDown(ui::Keycode code, int16_t value);
  void OnClose() final;

  bool Parse(const char *path);

  void RenderScrubberMenu();
  void RenderTextureMenu();
  void FormatTooltip(const hw::holly::PolyParam *param,
                     const renderer::Surface *surf);
  void FormatTooltip(const hw::holly::VertexParam *param,
                     const renderer::Vertex *vert, int vertex_type);
  void RenderContextMenu();

  void CopyCommandToContext(const hw::holly::TraceCommand *cmd,
                            hw::holly::TileContext *ctx);
  void PrevContext();
  void NextContext();
  void ResetContext();

  void PrevParam();
  void NextParam();
  void ResetParam();

  ui::Window &window_;
  renderer::Backend &rb_;
  TraceTextureCache texcache_;
  hw::holly::TileRenderer tile_renderer_;
  hw::holly::TileContext tctx_;
  hw::holly::TileRenderContext rctx_;

  bool running_;
  hw::holly::TraceReader trace_;

  bool hide_params_[hw::holly::TA_NUM_PARAMS];
  int current_frame_, num_frames_;
  hw::holly::TraceCommand *current_cmd_;
  int current_offset_;
};
}
}

#endif