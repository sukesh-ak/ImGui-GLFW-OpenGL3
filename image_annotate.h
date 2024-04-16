#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <vector>

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

struct BoundingBox {
    ImVec2 start;
    ImVec2 end;
    bool selected;
};

class ImageAnnotation {
public:
    //ImageAnnotation() : image_data(nullptr), imageWidth(0), imageHeight(0) {}

    ~ImageAnnotation() {
        stbi_image_free(image_data);
    }

    void loadImage(const char* filename) {
        bool ret = LoadTextureFromFile(filename);//,&image_texture,&imageWidth, &imageHeight);
        IM_ASSERT(ret);
    }

    void draw() {
        if (image_texture) {
            ImGui::Image((void*)(intptr_t)(image_texture), ImVec2(imageWidth, imageHeight));

/* START */

        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            BoundingBox newBox;
            newBox.start = ImGui::GetMousePos();
            newBox.end = newBox.start;
            boundingBoxes.push_back(newBox);
            is_drawing_boundingboxes = true;
        }

        if (is_drawing_boundingboxes && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            boundingBoxes.back().end = ImGui::GetMousePos();
        }

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            is_drawing_boundingboxes = false;
        }

        // Check for hover over existing bounding boxes
        hoveredBoxIndex = -1;
        for (size_t i = 0; i < boundingBoxes.size(); ++i) {
            const BoundingBox& box = boundingBoxes[i];
            if (ImGui::IsMouseHoveringRect(box.start, box.end) && !is_drawing_boundingboxes) {
                hoveredBoxIndex = static_cast<int>(i);
                break;
            }
        }

        // Draw all bounding boxes
        for (size_t i = 0; i < boundingBoxes.size(); ++i) {
            const BoundingBox& box = boundingBoxes[i];
            ImGui::GetWindowDrawList()->AddRect(box.start, box.end, IM_COL32(255, 255, 0, 255));
            if (static_cast<int>(i) == hoveredBoxIndex) {
                ImGui::GetWindowDrawList()->AddCircleFilled(box.start, 4.0f, IM_COL32(255, 0, 0, 255));
                ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(box.end.x, box.start.y), 4.0f, IM_COL32(255, 0, 0, 255));
                ImGui::GetWindowDrawList()->AddCircleFilled(box.end, 4.0f, IM_COL32(255, 0, 0, 255));
                ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(box.start.x, box.end.y), 4.0f, IM_COL32(255, 0, 0, 255));
                ImGui::Text("Bounding Box = (%f,%f) - (%f,%f)", box.start.x,box.start.y,box.end.x,box.end.y);
            }
        }
        

/* END */
        }
    }

    int GetHeight() {
        return imageHeight;
    }

    int GetWidth() {
        return imageWidth;
    }

private:
    // Function to render ImGui interface
    // Simple helper function to load an image into a OpenGL texture with common settings
    bool LoadTextureFromFile(const char* filename)
    {
        //unsigned char* 
        image_data = stbi_load(filename, &imageWidth, &imageHeight, NULL, 4);
        if (image_data == NULL)
            return false;

        // Create a OpenGL texture identifier
        //GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

        // Upload pixels into texture
    #if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    #endif
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        
        //boundingBoxes.clear();
        return true;
    }


    unsigned char* image_data;
    int imageWidth=0, imageHeight=0, imageChannels=0;
    std::vector<BoundingBox> boundingBoxes;
    int hoveredBoxIndex = -1;
    int selectedBoxIndex = -1;
    GLuint image_texture=0;

    bool is_drawing_boundingboxes;
};
