#include "HelloTriangleApplication.h"

void HelloTriangleApplication::run()
{
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}


static void framebufferResiceCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

void HelloTriangleApplication::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	pWindow = glfwCreateWindow(WIDTH, HEIGHT, "HelloTriangleApplication", nullptr, nullptr);
	glfwSetWindowUserPointer(pWindow, this); 
	glfwSetFramebufferSizeCallback(pWindow, framebufferResiceCallback);
}

void HelloTriangleApplication::initVulkan()
{
	createAndExecuteShaderBat();
	createInstance();
	setupDebugMessenger();
	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapChain();
	createImageViews();
	createRenderPass();
	createDescriptorLayout();
	createGraphicsPipeline();
	createFramebuffers();
	createCommandPool();
	createVertexBuffer();
	createIndexBuffer(); 
	createUniformBuffers();
	createDescriptorPool(); 
	createDescriptorSets(); 
	createCommandBuffers();
	createSyncObjects();  
}


void HelloTriangleApplication::mainLoop()
{
	while (!glfwWindowShouldClose(pWindow)) 
	{
		processInput();
		glfwPollEvents();
		drawFrame();
	}

	vkDeviceWaitIdle(logicalDevice);
}

void HelloTriangleApplication::cleanup()
{
	cleanupSwapchain();

	vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);

	vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroyBuffer(logicalDevice, uniformBuffers[i], nullptr);
		vkFreeMemory(logicalDevice, uniformBuffersMemory[i], nullptr);
	}

	vkDestroyDescriptorSetLayout(logicalDevice, descriptorSetLayout, nullptr);

	vkDestroyBuffer(logicalDevice, indexBuffer, nullptr);
	vkFreeMemory(logicalDevice, indexBufferMemory, nullptr);

	vkDestroyBuffer(logicalDevice, vertexBuffer, nullptr);
	vkFreeMemory(logicalDevice, vertexBufferMemory, nullptr);

	vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(logicalDevice, pipelineLayout, nullptr);

	vkDestroyRenderPass(logicalDevice, renderPass, nullptr);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
	}

	vkDestroyCommandPool(logicalDevice, commandPool, nullptr);

	vkDestroyDevice(logicalDevice, nullptr);

	if (enableValidationLayers) { 
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(pWindow);

	glfwTerminate();
}

void HelloTriangleApplication::processInput()
{
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(pWindow, true);
	}
}

void HelloTriangleApplication::createInstance()
{
	if (enableValidationLayers && !checkValidationLayerSupport()) 
	{
		throw std::runtime_error("Fail: validation layers requested, but not available!");
	}
	else {
		std::cout << "Success: found requested validation layers!" << std::endl;
	}

	VkApplicationInfo appInfo{}; 
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; 
	appInfo.pApplicationName = "Hello Triangle"; 
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); 
	appInfo.pEngineName = "No Engine"; 
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); 
	appInfo.apiVersion = VK_API_VERSION_1_0;


	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (enableValidationLayers) 
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else 
	{
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}


	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance); 

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vkinstance!");
	}
	else {
		std::cout << "" << std::endl;
		std::cout << "Success: created vkinstance!" << std::endl;
		std::cout << "Instance Create Info Type: " << createInfo.sType << std::endl;
		std::cout << "Instance Application Name: " << appInfo.pApplicationName << std::endl;
		std::cout << "Instance Type: " << appInfo.sType << std::endl;
		std::cout << "Instance Engine Name: " << appInfo.pEngineName << std::endl;   
	}

}

bool HelloTriangleApplication::checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

void HelloTriangleApplication::createAndExecuteShaderBat()
{
	std::ofstream batch; 
	batch.open("shaderbatchfile.bat", std::ios::out);

	batch << "glslc.exe shaders/shader.vert -o shaders/vert.spv\n";
	batch << "glslc.exe shaders/shader.frag -o shaders/frag.spv\n";
	//batch << "pause\n";

	batch.close();
	
	system("shaderbatchfile.bat");
	std::filesystem::remove("shaderbatchfile.bat"); 
}

std::vector<const char*> HelloTriangleApplication::getRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL HelloTriangleApplication::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

void HelloTriangleApplication::setupDebugMessenger()
{
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) { 
		throw std::runtime_error("failed to set up debug messenger!");
	}
	else {
		std::cout << "" << std::endl;
		std::cout << "Success: debug messenger was set up!" << std::endl;
	}
}

void HelloTriangleApplication::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

VkResult HelloTriangleApplication::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void HelloTriangleApplication::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

//scoring system to check which GraphicsCard is most suitable
int HelloTriangleApplication::rateDeviceSuitability(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	int score = 0;

	// Discrete GPUs have a significant performance advantage
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	// Maximum possible size of textures affects graphics quality
	score += deviceProperties.limits.maxImageDimension2D;

	// Application can't function without geometry shaders
	if (!deviceFeatures.geometryShader) {
		return 0;
	}

	return score;
}

//check if any advice is suitable 
bool HelloTriangleApplication::isDeviceSuitable(VkPhysicalDevice device)
{
	//return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;

	QueueFamilyIndices indices = findQueueFamilies(device, true);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	//needs to have at least one supported image format + one supported presentation mode
	bool swapChainAdequate = false;
	if (extensionsSupported) 
	{ 
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device); 
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty(); 
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

bool HelloTriangleApplication::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount); 
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end()); 

	for (const auto& extension : availableExtensions) { 
		requiredExtensions.erase(extension.extensionName); 
	}

	return requiredExtensions.empty();
}

void HelloTriangleApplication::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, true);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos; 
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() }; 

	std::cout << "The " << indices.graphicsFamily.value() << "th QueueFamily supports every required QueueFlag for graphical needs!" << std::endl;
	std::cout << "The " << indices.presentFamily.value() << "th QueueFamily supports presentation to a surface!" << std::endl;


	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) { 
		VkDeviceQueueCreateInfo queueCreateInfo{}; 
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; 
		queueCreateInfo.queueFamilyIndex = queueFamily; 
		queueCreateInfo.queueCount = 1; 
		queueCreateInfo.pQueuePriorities = &queuePriority; 
		queueCreateInfos.push_back(queueCreateInfo); 
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data(); 

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create logical device!");
	}
	else {
		std::cout << "" << std::endl;
		std::cout << "Success: created logical device!" << std::endl;
		std::cout << "Enabled Extension Count:  " << createInfo.enabledExtensionCount << std::endl;
		std::cout << "Extension Names: " << *createInfo.ppEnabledExtensionNames << std::endl;
		std::cout << "Enabled Validation Layer Count: " << createInfo.enabledLayerCount << std::endl; 
		if (createInfo.enabledLayerCount > 0) {
			std::cout << "Layer Names: " << *createInfo.ppEnabledLayerNames << std::endl;
		}
		std::cout << "Different QueueFamilies Count: " << createInfo.queueCreateInfoCount << std::endl;

	}

	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
}

void HelloTriangleApplication::createSurface()
{
	/*VkWin32SurfaceCreateInfoKHR createInfo{}; 
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = glfwGetWin32Window(pWindow);
	createInfo.hinstance = GetModuleHandle(nullptr);
	
	if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) 
	{
		throw std::runtime_error("window surface creation has failed!");
	}*/

	if (glfwCreateWindowSurface(instance, pWindow, nullptr, &surface) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create window surface!");
	}
	else {
		std::cout << "" << std::endl;
		std::cout << "Success: created window surface!" << std::endl;
	}
}

SwapChainSupportDetails HelloTriangleApplication::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	uint32_t formatCount; 
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr); 

	if (formatCount != 0) { 
		details.formats.resize(formatCount); 
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data()); 
	}

	uint32_t presentModeCount; 
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr); 

	if (presentModeCount != 0) { 
		details.presentModes.resize(presentModeCount); 
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data()); 
	}

	return details;
}

VkSurfaceFormatKHR HelloTriangleApplication::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats) 
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR HelloTriangleApplication::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D HelloTriangleApplication::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
#undef max
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(pWindow, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

void HelloTriangleApplication::createSwapChain()
{
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice); 

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats); 
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes); 
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities); 

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1; 

	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) 
	{ 
		imageCount = swapChainSupport.capabilities.maxImageCount; 
	}

	VkSwapchainCreateInfoKHR createInfo{}; 
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR; 
	createInfo.surface = surface; 

	createInfo.minImageCount = imageCount; 
	createInfo.imageFormat = surfaceFormat.format; 
	createInfo.imageColorSpace = surfaceFormat.colorSpace; 
	createInfo.imageExtent = extent; 
	createInfo.imageArrayLayers = 1; 
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; 

	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, false); 
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() }; 

	if (indices.graphicsFamily != indices.presentFamily) { 
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; 
		createInfo.queueFamilyIndexCount = 2; 
		createInfo.pQueueFamilyIndices = queueFamilyIndices; 
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; 
		createInfo.queueFamilyIndexCount = 0; // Optional 
		createInfo.pQueueFamilyIndices = nullptr; // Optional 
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform; 
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create swap chain!");
	}
	else {
		std::cout << "" << std::endl; 
		std::cout << "Success: created swap chain!" << std::endl; 
	}

	vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format; 
	swapChainExtent = extent; 
}

void HelloTriangleApplication::createImageViews()
{
	swapChainImageViews.resize(swapChainImages.size()); 

	for (size_t i = 0; i < swapChainImages.size(); i++) {
		VkImageViewCreateInfo createInfo{};

		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO; 
		createInfo.image = swapChainImages[i]; 

		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainImageFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY; 
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY; 
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY; 
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY; 

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; 
		createInfo.subresourceRange.baseMipLevel = 0; 
		createInfo.subresourceRange.levelCount = 1; 
		createInfo.subresourceRange.baseArrayLayer = 0; 
		createInfo.subresourceRange.layerCount = 1; 

		if (vkCreateImageView(logicalDevice, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image views!");
		}
		else {
			std::cout << "" << std::endl;
			std::cout << "Success: created image view number: " << i << "!" << std::endl;
		}

	}

}

static std::vector<char> readFile(const std::string &filename) 
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

void HelloTriangleApplication::createGraphicsPipeline()
{
	auto vertShaderCode = readFile("shaders/vert.spv");
	auto fragShaderCode = readFile("shaders/frag.spv");

	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo }; 


	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; 
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data(); 


	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;


	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;


	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;	//Insert VK_POLYGON_MODE_LINE to create a wireframe view (needs an additional GPU feature)
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;	//Which faces should be culled
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; //How are the front faces determined 
	rasterizer.depthBiasEnable = VK_FALSE;


	VkPipelineMultisampleStateCreateInfo multisampling{};  
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO; 
	multisampling.sampleShadingEnable = VK_FALSE; 
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; 


	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;


	VkPipelineColorBlendStateCreateInfo colorBlending{}; 
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO; 
	colorBlending.logicOpEnable = VK_FALSE; 
	colorBlending.logicOp = VK_LOGIC_OP_COPY; 
	colorBlending.attachmentCount = 1; 
	colorBlending.pAttachments = &colorBlendAttachment; 
	colorBlending.blendConstants[0] = 0.0f; 
	colorBlending.blendConstants[1] = 0.0f; 
	colorBlending.blendConstants[2] = 0.0f; 
	colorBlending.blendConstants[3] = 0.0f;


	std::vector<VkDynamicState> dynamicStates = {
	VK_DYNAMIC_STATE_VIEWPORT,
	VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();


	VkPipelineLayoutCreateInfo pipelineLayoutInfo{}; 
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1; 
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

	if (vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
	else {
		std::cout << "" << std::endl;
		std::cout << "Success: created pipeline layout " << std::endl;
	}


	VkGraphicsPipelineCreateInfo pipelineInfo{}; 
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO; 
	pipelineInfo.stageCount = 2; 
	pipelineInfo.pStages = shaderStages; 

	pipelineInfo.pVertexInputState = &vertexInputInfo; 
	pipelineInfo.pInputAssemblyState = &inputAssembly; 
	pipelineInfo.pViewportState = &viewportState; 
	pipelineInfo.pRasterizationState = &rasterizer; 
	pipelineInfo.pMultisampleState = &multisampling; 
	pipelineInfo.pColorBlendState = &colorBlending; 
	pipelineInfo.pDynamicState = &dynamicState; 

	pipelineInfo.layout = pipelineLayout; 

	pipelineInfo.renderPass = renderPass; 
	pipelineInfo.subpass = 0; 
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; 

	if (vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}
	else {
		std::cout << "" << std::endl;
		std::cout << "Success: created graphics pipeline " << std::endl;
	}



	vkDestroyShaderModule(logicalDevice, fragShaderModule, nullptr);
	vkDestroyShaderModule(logicalDevice, vertShaderModule, nullptr);
}

void HelloTriangleApplication::createRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; 
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; 
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; 
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; 


	VkAttachmentReference colorAttachmentRef{}; 
	colorAttachmentRef.attachment = 0; 
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; 


	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1; 
	subpass.pColorAttachments = &colorAttachmentRef; 


	VkSubpassDependency dependency{}; 
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL; 
	dependency.dstSubpass = 0; 
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; 
	dependency.srcAccessMask = 0; 
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; 
	dependency.srcAccessMask = 0; 
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; 
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; 


	VkRenderPassCreateInfo renderPassInfo{}; 
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO; 
	renderPassInfo.attachmentCount = 1; 
	renderPassInfo.pAttachments = &colorAttachment; 
	renderPassInfo.subpassCount = 1; 
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1; 
	renderPassInfo.pDependencies = &dependency; 

	if (vkCreateRenderPass(logicalDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}
	else {
		std::cout << "" << std::endl;
		std::cout << "Success: created render pass " << std::endl;
	}
}

void HelloTriangleApplication::createDescriptorLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional 

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(logicalDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
	else {
		std::cout << "" << std::endl;
		std::cout << "Success: created descriptor set layout" << std::endl;
	}

}

VkShaderModule HelloTriangleApplication::createShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;

	if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create shader module!");
	}
	else {
		std::cout << "" << std::endl;
		std::cout << "Success: created shader module" << std::endl;
	}

	return shaderModule;
}

void HelloTriangleApplication::createFramebuffers()
{
	swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		VkImageView attachments[] = {
			swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer number: " + i);
		}
		else {
			std::cout << "" << std::endl;
			std::cout << "Success: created frame buffer number: " << i << std::endl;
		}
	}
}

void HelloTriangleApplication::createCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice, false);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
	else {
		std::cout << "" << std::endl;
		std::cout << "Success: created command pool" << std::endl;
	}
}

void HelloTriangleApplication::createVertexBuffer()
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	VkBuffer stagingBuffer; 
	VkDeviceMemory stagingBufferMemory; 
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory); 

	void* data;
	vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferMemory);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}

uint32_t HelloTriangleApplication::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{ 
	VkPhysicalDeviceMemoryProperties memProperties; 
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties); 

	std::cout << "" << std::endl;
	std::cout << "Memory Type Count: " << memProperties.memoryTypeCount << std::endl;
	std::cout << "Memory Heap Count: " << memProperties.memoryHeapCount << std::endl;

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) { 
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i; 
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void HelloTriangleApplication::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory!");
	}
	else {
		std::cout << "" << std::endl;
		std::cout << "Success: allocated vertex buffer memory" << std::endl;
	}

	vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0);
}

void HelloTriangleApplication::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer); 

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
}

void HelloTriangleApplication::createIndexBuffer()
{
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferMemory);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

	copyBuffer(stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}

void HelloTriangleApplication::createUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
			uniformBuffers[i], uniformBuffersMemory[i]);

		vkMapMemory(logicalDevice, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
	}
}

void HelloTriangleApplication::createDescriptorPool() 
{
	VkDescriptorPoolSize poolSize{}; 
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; 
	poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	 
	VkDescriptorPoolCreateInfo poolInfo{}; 
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO; 
	poolInfo.poolSizeCount = 1; 
	poolInfo.pPoolSizes = &poolSize; 
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
	else {
		std::cout << "" << std::endl;
		std::cout << "Success: created descriptor pool" << std::endl;
	}
}

void HelloTriangleApplication::createDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(MAX_FRAMES_IN_FLIGHT); 

	if (vkAllocateDescriptorSets(logicalDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
	else {
		std::cout << "" << std::endl;
		std::cout << "Success: allocated descriptor sets" << std::endl;
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(logicalDevice, 1, &descriptorWrite, 0, nullptr);
	}
}

void HelloTriangleApplication::createCommandBuffers()
{
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);   

	VkCommandBufferAllocateInfo allocInfo{}; 
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO; 
	allocInfo.commandPool = commandPool; 
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; 
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size(); 

	if (vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
	else {
		std::cout << "" << std::endl;
		std::cout << "Success: created command buffer" << std::endl;
	}
}

void HelloTriangleApplication::createSyncObjects()
{
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{}; 
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO; 

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create semaphores/fences!");
		}
		else {
			std::cout << "" << std::endl;
			std::cout << "Success: created semaphores/fences" << std::endl;
		}
	}

}

void HelloTriangleApplication::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{}; 
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO; 
	beginInfo.flags = 0; // Optional 
	beginInfo.pInheritanceInfo = nullptr; // Optional 

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) { 
		throw std::runtime_error("failed to begin recording command buffer!");
	}
	 
	VkRenderPassBeginInfo renderPassInfo{}; 
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO; 
	renderPassInfo.renderPass = renderPass; 
	renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];

	renderPassInfo.renderArea.offset = { 0, 0 }; 
	renderPassInfo.renderArea.extent = swapChainExtent; 

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} }; 
	renderPassInfo.clearValueCount = 1; 
	renderPassInfo.pClearValues = &clearColor; 

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE); 


	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline); 

	VkViewport viewport{}; 
	viewport.x = 0.0f; 
	viewport.y = 0.0f; 
	viewport.width = static_cast<float>(swapChainExtent.width); 
	viewport.height = static_cast<float>(swapChainExtent.height); 
	viewport.minDepth = 0.0f; 
	viewport.maxDepth = 1.0f; 
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport); 

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


	VkBuffer vertexBuffers[] = { vertexBuffer }; 
	VkDeviceSize offsets[] = { 0 }; 
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets); 

	vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);


	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, 
		pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);

	//vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
		
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0); //DRAW COMMAND   


	vkCmdEndRenderPass(commandBuffer); 

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) { 
		throw std::runtime_error("failed to record command buffer!"); 
	} 
}

void HelloTriangleApplication::drawFrame()
{
	vkWaitForFences(logicalDevice, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex; 
	VkResult result = vkAcquireNextImageKHR(logicalDevice, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapchain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	updateUniformBuffer(currentFrame);

	vkResetFences(logicalDevice, 1, &inFlightFences[currentFrame]);

	vkResetCommandBuffer(commandBuffers[currentFrame], 0);
	recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

	VkSubmitInfo submitInfo{}; 
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO; 

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };								//what to wait for 
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };		//when to wait
	submitInfo.waitSemaphoreCount = 1; 
	submitInfo.pWaitSemaphores = waitSemaphores;												//tell submitInfo what to wait for
	submitInfo.pWaitDstStageMask = waitStages; 
	submitInfo.commandBufferCount = 1; 
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };								//which semaphore to signal when finished rendering

	submitInfo.signalSemaphoreCount = 1; 
	submitInfo.pSignalSemaphores = signalSemaphores;											//tell submitInfo who to tell when finished

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!"); 
	} 


	VkPresentInfoKHR presentInfo{}; 
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR; 
	presentInfo.waitSemaphoreCount = 1; 
	presentInfo.pWaitSemaphores = signalSemaphores; 


	VkSwapchainKHR swapChains[] = { swapChain }; 
	presentInfo.swapchainCount = 1; 
	presentInfo.pSwapchains = swapChains; 
	presentInfo.pImageIndices = &imageIndex; 
	presentInfo.pResults = nullptr; // Optional 

	result = vkQueuePresentKHR(presentQueue, &presentInfo); 

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
		recreateSwapchain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void HelloTriangleApplication::updateUniformBuffer(uint32_t currentImage)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f); 

	ubo.proj[1][1] *= -1; 

	memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo)); 
}

void HelloTriangleApplication::recreateSwapchain()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(pWindow, &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(pWindow, &width, &height);
		glfwWaitEvents(); 
	}

	vkDeviceWaitIdle(logicalDevice);

	cleanupSwapchain();

	createSwapChain(); 
	createImageViews(); 
	createFramebuffers(); 
}

void HelloTriangleApplication::cleanupSwapchain()
{
	for (auto framebuffer : swapChainFramebuffers) {
		vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
	}

	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(logicalDevice, imageView, nullptr);
	}

	vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
}

void HelloTriangleApplication::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)  
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}
	else{
		std::cout << "" << std::endl;
		std::cout << "Success: found GPUs with vulkan support!" << std::endl;
		std::vector<VkPhysicalDevice> devices(deviceCount);  
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data()); 

		for (auto& device : devices) {
			VkPhysicalDeviceProperties deviceProperties; 
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			std::cout << "Device Name: " << deviceProperties.deviceName <<std::endl;
		}
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);  
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());  

	//std::multimap<int, VkPhysicalDevice> candidates;

	//for (const VkPhysicalDevice device : devices) {
	//	int score = rateDeviceSuitability(device);
	//	candidates.insert(std::make_pair(score, device));
	//	std::cout << "Device: " << device << " has a score of: " << score << std::endl;
	//}

	//// Check if the best candidate is suitable at all
	//if (candidates.rbegin()->first > 0) {
	//	physicalDevice = candidates.rbegin()->second;
	//}
	//else {
	//	throw std::runtime_error("failed to find a suitable GPU!");
	//}

	for (const auto& device : devices) 
	{
		if (isDeviceSuitable(device)) 
		{
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) 
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}
	else {
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);
		std::cout << "" << std::endl;
		std::cout << "Success: found suitable GPU! : " << properties.deviceName << std::endl;
	}


}

QueueFamilyIndices HelloTriangleApplication::findQueueFamilies(VkPhysicalDevice device, bool debugQueueFamilies)
{
	QueueFamilyIndices indices; 

	uint32_t queueFamilyCount; 
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	if (debugQueueFamilies) 
	{
		for (auto& queueFamily : queueFamilies) {
			std::cout << "Queue Count in Family: " << queueFamily.queueCount << std::endl;
			std::cout << "Queue Flags:" << std::endl;

			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				std::cout << "| VK_QUEUE_GRAPHICS_BIT " << "-> Graphics Operations" << std::endl;
			}
			if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				std::cout << "| VK_QUEUE_COMPUTE_BIT " << "-> Compute Operations" << std::endl;
			}
			if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				std::cout << "| VK_QUEUE_TRANSFER_BIT " << "-> Transfer Operations" << std::endl;
			}
			if (queueFamily.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
			{
				std::cout << "| VK_QUEUE_SPARSE_BINDING_BIT " << "-> Sparse Memory Management" << std::endl;
			}
			if (queueFamily.queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR)
			{
				std::cout << "| VK_QUEUE_VIDEO_DECODE_BIT_KHR " << "-> Video Decode Operations" << std::endl;
			}
			if (queueFamily.queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR)
			{
				std::cout << "| VK_QUEUE_VIDEO_ENCODE_BIT_KHR " << "-> Video Encode Operations" << std::endl;
			}
			if (queueFamily.queueFlags & VK_QUEUE_OPTICAL_FLOW_BIT_NV)
			{
				std::cout << "| VK_QUEUE_OPTICAL_FLOW_BIT_NV " << "-> Optical Flow Operations" << std::endl;
			}
			if (queueFamily.queueFlags & VK_QUEUE_PROTECTED_BIT)
			{
				std::cout << "| VK_QUEUE_PROTECTED_BIT " << "-> VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT ???" << std::endl;
			}
		}
	}
	

	uint32_t i = 0; 
	for (const auto& queueFamily : queueFamilies) { 

		bool hasQueueFlags = true;
		for (auto& queueFlag : neededQueueFlags) {
			if (queueFamily.queueFlags & queueFlag) {

			}
			else {
				hasQueueFlags = false;
			}
		}

		if (hasQueueFlags) { 
			indices.graphicsFamily = i; 
		} 

		VkBool32 presentSupport = false; 
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (presentSupport) { 
			indices.presentFamily = i; 
		}

		if (indices.isComplete()) { 
			break;
		}

		i++; 
	}

	return indices;
}






