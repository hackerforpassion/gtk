#include "config.h"

#include "gskvulkancrossfadepipelineprivate.h"

struct _GskVulkanCrossFadePipeline
{
  GObject parent_instance;
};

typedef struct _GskVulkanCrossFadeInstance GskVulkanCrossFadeInstance;

struct _GskVulkanCrossFadeInstance
{
  float rect[4];
  float tex_rect[4];
  float progress;
};

G_DEFINE_TYPE (GskVulkanCrossFadePipeline, gsk_vulkan_cross_fade_pipeline, GSK_TYPE_VULKAN_PIPELINE)

static const VkPipelineVertexInputStateCreateInfo *
gsk_vulkan_cross_fade_pipeline_get_input_state_create_info (GskVulkanPipeline *self)
{
  static const VkVertexInputBindingDescription vertexBindingDescriptions[] = {
      {
          .binding = 0,
          .stride = sizeof (GskVulkanCrossFadeInstance),
          .inputRate = VK_VERTEX_INPUT_RATE_INSTANCE
      }
  };
  static const VkVertexInputAttributeDescription vertexInputAttributeDescription[] = {
      {
          .location = 0,
          .binding = 0,
          .format = VK_FORMAT_R32G32B32A32_SFLOAT,
          .offset = 0,
      },
      {
          .location = 1,
          .binding = 0,
          .format = VK_FORMAT_R32G32B32A32_SFLOAT,
          .offset = G_STRUCT_OFFSET (GskVulkanCrossFadeInstance, tex_rect),
      },
      {
          .location = 2,
          .binding = 0,
          .format = VK_FORMAT_R32_SFLOAT,
          .offset = G_STRUCT_OFFSET (GskVulkanCrossFadeInstance, progress),
      }
  };
  static const VkPipelineVertexInputStateCreateInfo info = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
      .vertexBindingDescriptionCount = G_N_ELEMENTS (vertexBindingDescriptions),
      .pVertexBindingDescriptions = vertexBindingDescriptions,
      .vertexAttributeDescriptionCount = G_N_ELEMENTS (vertexInputAttributeDescription),
      .pVertexAttributeDescriptions = vertexInputAttributeDescription
  };

  return &info;
}

static void
gsk_vulkan_cross_fade_pipeline_finalize (GObject *gobject)
{
  //GskVulkanCrossFadePipeline *self = GSK_VULKAN_BLUR_PIPELINE (gobject);

  G_OBJECT_CLASS (gsk_vulkan_cross_fade_pipeline_parent_class)->finalize (gobject);
}

static void
gsk_vulkan_cross_fade_pipeline_class_init (GskVulkanCrossFadePipelineClass *klass)
{
  GskVulkanPipelineClass *pipeline_class = GSK_VULKAN_PIPELINE_CLASS (klass);

  G_OBJECT_CLASS (klass)->finalize = gsk_vulkan_cross_fade_pipeline_finalize;

  pipeline_class->get_input_state_create_info = gsk_vulkan_cross_fade_pipeline_get_input_state_create_info;
}

static void
gsk_vulkan_cross_fade_pipeline_init (GskVulkanCrossFadePipeline *self)
{
}

GskVulkanPipeline *
gsk_vulkan_cross_fade_pipeline_new (GdkVulkanContext        *context,
                                    VkPipelineLayout         layout,
                                    const char              *shader_name,
                                    VkRenderPass             render_pass)
{
  return gsk_vulkan_pipeline_new (GSK_TYPE_VULKAN_CROSS_FADE_PIPELINE, context, layout, shader_name, render_pass);
}

gsize
gsk_vulkan_cross_fade_pipeline_count_vertex_data (GskVulkanCrossFadePipeline *pipeline)
{
  return sizeof (GskVulkanCrossFadeInstance);
}

void
gsk_vulkan_cross_fade_pipeline_collect_vertex_data (GskVulkanCrossFadePipeline *pipeline,
                                                    guchar                *data,
                                                    const graphene_rect_t *rect,
                                                    double                 progress)
{
  GskVulkanCrossFadeInstance *instance = (GskVulkanCrossFadeInstance *) data;

  instance->rect[0] = rect->origin.x;
  instance->rect[1] = rect->origin.y;
  instance->rect[2] = rect->size.width;
  instance->rect[3] = rect->size.height;
  instance->tex_rect[0] = 0.0;
  instance->tex_rect[1] = 0.0;
  instance->tex_rect[2] = 1.0;
  instance->tex_rect[3] = 1.0;
  instance->progress = progress;
}

gsize
gsk_vulkan_cross_fade_pipeline_draw (GskVulkanCrossFadePipeline *pipeline,
                                     VkCommandBuffer        command_buffer,
                                     gsize                  offset,
                                     gsize                  n_commands)
{
  vkCmdDraw (command_buffer,
             6, n_commands,
             0, offset);

  return n_commands;
}