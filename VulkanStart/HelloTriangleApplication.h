#ifndef HELLO_TRIANGLE_APPLICATION_H

#define HELLO_TRIANGLE_APPLICATION_H

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#define VK_ENABLE_BETA_EXTENSIONS 

#include <vulkan/vulkan.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>	// Necessary for std::clamp
#include <vector>
#include <cstring>
#include <cstdlib>		// Necessary for uint32_t
#include <cstdint>
#include <limits>		// Necessary for std::numeric_limits
#include <array>
#include <optional>
#include <set>


#include <filesystem>


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#define GLM_FORCE_RADIANS 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
 
#include "imgui/imgui.h" 
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_vulkan.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0; 
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
		attributeDescriptions[0].binding = 0; 
		attributeDescriptions[0].location = 0; 
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT; 
		attributeDescriptions[0].offset = offsetof(Vertex, pos); 

		attributeDescriptions[1].binding = 0; 
		attributeDescriptions[1].location = 1; 
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT; 
		attributeDescriptions[1].offset = offsetof(Vertex, color); 

		return attributeDescriptions;
	}
};
const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}

};

const std::vector<uint32_t> indices = {
	0, 1, 2, 2, 3, 0
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const int WIDTH = 1920;
const int HEIGHT = 1080;

const float RED = 0.35f;
const float GREEN = 0.0f;
const float BLUE = 1.0f;
const float ALPHA = 1.0f;

const std::vector<VkQueueFlagBits> neededQueueFlags = { VK_QUEUE_GRAPHICS_BIT };

class HelloTriangleApplication
{
public:
	void run();

	bool framebufferResized = false;

private:
	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();


	void processInput();

	void createAndExecuteShaderBat();

	void createInstance();
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();

	void setupDebugMessenger();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator);

	void pickPhysicalDevice();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, bool debugQueueFamilies);
	int rateDeviceSuitability(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	void createLogicalDevice();

	void createSurface();

	void createSwapChain();
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	void createImageViews();

	void createRenderPass();

	void createDescriptorLayout();

	void createGraphicsPipeline();
	VkShaderModule createShaderModule(const std::vector<char>& code);

	void createFramebuffers();

	void createCommandPool();

	void createVertexBuffer();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void createIndexBuffer(); 

	void createUniformBuffers();

	void createDescriptorPool();
	void createDescriptorSets();

	void createCommandBuffers();

	void createSyncObjects();

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	void drawFrame();
	void updateUniformBuffer(uint32_t currentImage);

	void recreateSwapchain();
	void cleanupSwapchain();

	GLFWwindow* pWindow = nullptr;
	VkInstance instance = NULL;
	VkDebugUtilsMessengerEXT debugMessenger = NULL;
	VkPhysicalDevice physicalDevice = NULL;
	VkDevice logicalDevice = NULL;
	VkQueue graphicsQueue = NULL;
	VkSurfaceKHR surface = NULL;
	VkQueue presentQueue = NULL;
	VkSwapchainKHR swapChain = NULL;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	VkRenderPass renderPass = NULL;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout = NULL;
	VkPipeline graphicsPipeline = NULL;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkCommandPool commandPool = NULL;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory; 
	std::vector<void*> uniformBuffersMapped;
	VkDescriptorPool descriptorPool; 
	std::vector<VkDescriptorSet> descriptorSets; 
	int currentFrame = 0;

};

#endif // !HELLO_TRIANGLE_APPLICATION_H



