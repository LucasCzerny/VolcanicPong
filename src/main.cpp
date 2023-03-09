#include "Dependencies.h"

#include "CustomAssert.h"

constexpr uint32_t WIDTH = 1280;
constexpr uint32_t HEIGHT = 720;

constexpr float ASPECT_RATIO = (float)WIDTH / HEIGHT;

constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

constexpr float PLAYER_WIDTH = 0.1f;
constexpr float PLAYER_HEIGHT = 0.6f;
constexpr float PLAYER_POSITION = 0.1f;
constexpr float PADDING = 0.005f;
constexpr float MOVEMENT_SPEED = 0.05f;

constexpr float BALL_SIZE = 0.1f;
constexpr float MIN_BALL_SPEED = 0.05f;
constexpr float MAX_BALL_SPEED = 0.15f;

struct QueueFamilyIndices
{
	uint32_t GraphicsFamily;
	bool HasGraphicsFamily = false;

	uint32_t PresentFamily;
	bool HasPresentFamily = false;

	bool IsComplete()
	{
		return HasGraphicsFamily && HasPresentFamily;
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR Capabilities;
	std::vector<VkSurfaceFormatKHR> SurfaceFormats;
	std::vector<VkPresentModeKHR> PresentModes;

	bool IsComplete()
	{
		return !SurfaceFormats.empty() && !PresentModes.empty();
	}
};

// PipelineLayout and RenderPass need to be set manually!
struct PipelineConfig
{
	PipelineConfig() = default;

	void UseDefaults();

	VkPipelineViewportStateCreateInfo ViewportInfo;
	VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
	VkPipelineRasterizationStateCreateInfo RasterizationInfo;
	VkPipelineMultisampleStateCreateInfo MultisampleInfo;
	VkPipelineColorBlendAttachmentState ColorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
	VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;
	std::vector<VkDynamicState> DynamicStateEnables;
	VkPipelineDynamicStateCreateInfo DynamicStateInfo;
	VkPipelineLayout PipelineLayout = nullptr;
	VkRenderPass RenderPass = nullptr;
	uint32_t Subpass = 0;
};

struct Vertex
{
	glm::vec2 Position;
	glm::vec3 Color;

	static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);

		// ???
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescriptions;
	}

	static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, Position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, Color);

		return attributeDescriptions;
	}
};

struct SyncObjects
{
	std::vector<VkSemaphore> ImageAvailableSemaphores;
	std::vector<VkSemaphore> RenderingFinishedSemaphores;
	std::vector<VkFence> InFlightFences;
	std::vector<VkFence> ImagesInFlight;
};

struct ShaderMatrices
{
	glm::mat4 Projection;
	glm::mat4 Transform;
};

GLFWwindow* CreateGlfwWindow();

VkDebugUtilsMessengerCreateInfoEXT GetDebugMessengerInfo();
static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

VkInstance CreateInstance(bool enableValidationLayers, const std::vector<const char*>& validationLayers);
bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers);

VkDebugUtilsMessengerEXT SetupDebugMessenger(VkInstance instance);
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

VkSurfaceKHR CreateSurface(VkInstance instance, GLFWwindow* window);

VkPhysicalDevice ChoosePhysicalDevice(VkInstance instance, VkSurfaceKHR surface, const std::vector<const char*>& deviceExtensions);
QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
bool SupportsRequiredExtensions(const VkPhysicalDevice& device, const std::vector<const char*>& deviceExtensions);
SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
bool SupportsAnisotropicSampling(const VkPhysicalDevice& device);
bool IsDeviceSuitable(const VkPhysicalDevice& device, const VkSurfaceKHR& surface, const std::vector<const char*>& deviceExtensions);

uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

VkDevice ChooseLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const QueueFamilyIndices& indices, const std::vector<const char*>& deviceExtensions);

VkQueue GetQueue(VkDevice device, uint32_t queueFamily, uint32_t queueIndex = 0);

VkCommandPool CreateCommandPool(VkDevice device, uint32_t queueFamilyIndex);

VkSurfaceFormatKHR ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availableModes);
VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

VkSwapchainKHR CreateSwapChain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR capabilities, VkExtent2D extent, VkSurfaceFormatKHR surfaceFormat, VkPresentModeKHR presentMode, const QueueFamilyIndices& queueIndices);

void GetSwapChainImages(VkDevice device, VkSwapchainKHR swapChain, std::vector<VkImage>& images);
void CreateImageViews(VkDevice device, const std::vector<VkImage>& swapChainImages, VkFormat imageFormat, std::vector<VkImageView>& imageViews);

VkFormat FindSupportedDepthFormat(VkPhysicalDevice physicalDevice);

void CreateDepthResources(VkDevice device, VkPhysicalDevice physicalDevice, VkMemoryPropertyFlags properties, VkFormat depthFormat, VkExtent2D swapChainExtent, uint32_t imageCount, std::vector<VkImage>& depthImages, std::vector<VkDeviceMemory>& depthMemory, std::vector<VkImageView>& depthImageViews);

VkRenderPass CreateRenderPass(VkDevice device, VkPhysicalDevice physicalDevice, VkFormat imageFormat, VkFormat depthFormat);

void CreateFramebuffers(VkDevice device, VkRenderPass renderPass, VkExtent2D swapChainExtent, uint32_t imageCount, const std::vector<VkImageView>& colorImageViews, const std::vector<VkImageView>& depthImageViews, std::vector<VkFramebuffer>& framebuffers);

void CreateSyncObjects(VkDevice device, uint32_t imageCount, SyncObjects& syncObjects);

VkPipelineLayout CreatePipelineLayout(VkDevice device);

VkPipeline CreatePipeline(VkDevice device, VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const fs::path& vertexShaderPath, const fs::path& fragmentShaderPath);
std::string ReadFile(const fs::path& filePath);
VkShaderModule CreateShaderModule(VkDevice device, const std::string& shaderSource);

VkBuffer CreateVertexBuffers(VkDevice device, VkPhysicalDevice physicalDevice, const std::vector<Vertex>& vertices, VkDeviceMemory& deviceMemory);

void CreateCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t imageCount, std::vector<VkCommandBuffer>& commandBuffers);

void RecordCommandBuffer(const std::array<glm::mat4, 3> transforms, VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, VkExtent2D swapChainExtent, VkRenderPass renderPass, VkPipeline pipeline, VkPipelineLayout layout, VkBuffer vertexBuffer, uint32_t vertexCount);

void DrawFrame(VkDevice device, VkSwapchainKHR swapChain, VkQueue graphicsQueue, VkQueue presentQueue, const std::vector<VkCommandBuffer>& commandBuffers, SyncObjects& syncObjects, const std::array<glm::mat4, 3>& transforms, const std::vector<VkFramebuffer>& framebuffers, VkExtent2D swapChainExtent, VkRenderPass renderPass, VkPipeline pipeline, VkPipelineLayout pipelineLayout, VkBuffer vertexBuffer, uint32_t verticesSize);
uint32_t AquireNextImage(VkDevice device, VkSwapchainKHR swapChain, SyncObjects& syncObjects, uint32_t currentFrame);
void SubmitCommandBuffers(VkDevice device, VkSwapchainKHR swapChain, VkQueue graphicsQueue, VkQueue presentQueue, VkCommandBuffer commandBuffer, SyncObjects& syncObjects, uint32_t imageIndex, uint32_t currentFrame);

std::array<glm::mat4, 3> CalculateTransforms(const std::array<glm::vec2, 3>& positions);
void MovePlayer(float& position, float amount);
int MoveBall(std::array<glm::vec2, 3>& positions, glm::vec2& direction);
void Bounce(const glm::vec2& surfaceNormal, glm::vec2& direction);

int main()
{
	ASSERT(glfwInit(), "Failed to initialize GLFW.");

	GLFWwindow* window = CreateGlfwWindow();

	#ifdef CONFIGURATION_DEBUG
		const bool enableValidationLayers = true;
	#else
		const bool enableValidationLayers = false;
	#endif

	std::vector<const char*> validationLayers(1);
	validationLayers[0] = "VK_LAYER_KHRONOS_validation";

	VkInstance instance = CreateInstance(enableValidationLayers, validationLayers);

	VkDebugUtilsMessengerEXT debugMessenger;
	if (enableValidationLayers)
	{
		debugMessenger = SetupDebugMessenger(instance);
	}

	// TODO describe
	VkSurfaceKHR surface = CreateSurface(instance, window);

	std::vector<const char*> deviceExtensions(1);
	deviceExtensions[0] = "VK_KHR_swapchain";

	VkPhysicalDevice physicalDevice = ChoosePhysicalDevice(instance, surface, deviceExtensions);

	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

	std::cout << "Using GPU: " << physicalDeviceProperties.deviceName << "\n";

	QueueFamilyIndices queueIndices = GetQueueFamilies(physicalDevice, surface);

	VkDevice logicalDevice = ChooseLogicalDevice(physicalDevice, surface, queueIndices, deviceExtensions);

	VkQueue graphicsQueue = GetQueue(logicalDevice, queueIndices.GraphicsFamily);
	VkQueue presentQueue = GetQueue(logicalDevice, queueIndices.PresentFamily);

	VkCommandPool commandPool = CreateCommandPool(logicalDevice, queueIndices.GraphicsFamily);

	SwapChainSupportDetails supportDetails = QuerySwapChainSupport(physicalDevice, surface);

	VkExtent2D swapChainExtent = ChooseExtent(supportDetails.Capabilities);
	VkSurfaceFormatKHR swapChainSurfaceFormat = ChooseSwapChainSurfaceFormat(supportDetails.SurfaceFormats);
	VkPresentModeKHR swapChainPresentMode = ChoosePresentMode(supportDetails.PresentModes);

	VkSwapchainKHR swapChain = CreateSwapChain(logicalDevice, physicalDevice, surface, supportDetails.Capabilities, swapChainExtent, swapChainSurfaceFormat, swapChainPresentMode, queueIndices);

	std::vector<VkImage> swapChainImages;
	GetSwapChainImages(logicalDevice, swapChain, swapChainImages);

	uint32_t swapChainImageCount = swapChainImages.size();

	std::vector<VkImageView> swapChainImageViews;
	CreateImageViews(logicalDevice, swapChainImages, swapChainSurfaceFormat.format, swapChainImageViews);

	VkFormat depthFormat = FindSupportedDepthFormat(physicalDevice);

	std::vector<VkImage> depthImages;
	std::vector<VkDeviceMemory> depthMemory;
	std::vector<VkImageView> depthImageViews;
	CreateDepthResources(logicalDevice, physicalDevice, NULL, depthFormat, swapChainExtent, swapChainImageCount, depthImages, depthMemory, depthImageViews);

	VkRenderPass renderPass = CreateRenderPass(logicalDevice, physicalDevice, swapChainSurfaceFormat.format, depthFormat);

	std::vector<VkFramebuffer> framebuffers;
	CreateFramebuffers(logicalDevice, renderPass, swapChainExtent, swapChainImageCount, swapChainImageViews, depthImageViews, framebuffers);

	SyncObjects syncObjects;
	CreateSyncObjects(logicalDevice, swapChainImageCount, syncObjects);

	VkPipelineLayout pipelineLayout = CreatePipelineLayout(logicalDevice);

	fs::path vertexShaderPath = "shaders/pong.vert.spv";
	fs::path fragmentShaderPath = "shaders/pong.frag.spv";
	VkPipeline pipeline = CreatePipeline(logicalDevice, renderPass, pipelineLayout, vertexShaderPath, fragmentShaderPath);

	static std::vector<Vertex> vertices = {
		{ { -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f } },
		{ {  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f } },
		{ {  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f } },
		{ {  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f } },
		{ { -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f } },
		{ { -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f } },
	};

	VkDeviceMemory vertexBufferMemory;
	VkBuffer vertexBuffer = CreateVertexBuffers(logicalDevice, physicalDevice, vertices, vertexBufferMemory);

	std::vector<VkCommandBuffer> commandBuffers;
	CreateCommandBuffers(logicalDevice, commandPool, swapChainImageCount, commandBuffers);

	std::array<glm::vec2, 3> positions = { {
		{ -ASPECT_RATIO + PLAYER_POSITION, 0.0f},
		{  ASPECT_RATIO - PLAYER_POSITION, 0.0f },
		{  0.0f, 0.0f },
	} };

	glm::vec2 ballDirection = { 1.0f, 1.0f };
	ballDirection = glm::normalize(ballDirection);

	unsigned int scores[2] = { 0 };

	bool shouldQuit = false;

	while (!glfwWindowShouldClose(window) && !shouldQuit)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			MovePlayer(positions[0].y, -MOVEMENT_SPEED);
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			MovePlayer(positions[0].y, MOVEMENT_SPEED);
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			MovePlayer(positions[1].y, -MOVEMENT_SPEED);
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			MovePlayer(positions[1].y, MOVEMENT_SPEED);
		}

		int scoringPlayer = MoveBall(positions, ballDirection);

		if (scoringPlayer)
		{
			scores[scoringPlayer - 1]++;

			std::cout << "\n";
			std::cout << "Player " << scoringPlayer << " scored a goal!\n";
			std::cout << "Score: " << scores[0] << " - " << scores[1] << "\n";
		}

		std::array<glm::mat4, 3> transforms = CalculateTransforms(positions);

		glfwPollEvents();
		DrawFrame(logicalDevice, swapChain, graphicsQueue, presentQueue, commandBuffers, syncObjects, transforms, framebuffers, swapChainExtent, renderPass, pipeline, pipelineLayout, vertexBuffer, vertices.size());

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			shouldQuit = true;

			std::cout << "\n";
			std::cout << "The game is over\n";
			std::cout << "Score: " << scores[0] << " - " << scores[1] << "\n";
		}
	}

	// Clean up

	vkQueueWaitIdle(graphicsQueue);
	vkQueueWaitIdle(presentQueue);

	vkFreeCommandBuffers(logicalDevice, commandPool, commandBuffers.size(), commandBuffers.data());

	vkDestroyBuffer(logicalDevice, vertexBuffer, nullptr);
	vkFreeMemory(logicalDevice, vertexBufferMemory, nullptr);

	for (auto imageView : swapChainImageViews)
	{
		vkDestroyImageView(logicalDevice, imageView, nullptr);
	}

	vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);

	for (uint32_t i = 0; i < depthImages.size(); i++)
	{
		vkDestroyImage(logicalDevice, depthImages[i], nullptr);
		vkFreeMemory(logicalDevice, depthMemory[i], nullptr);
		vkDestroyImageView(logicalDevice, depthImageViews[i], nullptr);
	}

	for (auto framebuffer : framebuffers)
	{
		vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
	}

	vkDestroyRenderPass(logicalDevice, renderPass, nullptr);

	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(logicalDevice, syncObjects.ImageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(logicalDevice, syncObjects.RenderingFinishedSemaphores[i], nullptr);
		vkDestroyFence(logicalDevice, syncObjects.InFlightFences[i], nullptr);
	}

	vkDestroySurfaceKHR(instance, surface, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();

	if (enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
}

GLFWwindow* CreateGlfwWindow()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Volcanic Pong", nullptr, nullptr);
	ASSERT(window, "Failed to create GLFW window.");

	return window;
}

VkDebugUtilsMessengerCreateInfoEXT GetDebugMessengerInfo()
{
	static VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

	// Call the callback for messages of any severity and any type
	debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;

	debugCreateInfo.pfnUserCallback = VulkanDebugCallback;

	return debugCreateInfo;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		std::cout << pCallbackData->pMessage << "\n";
	}

	return VK_FALSE;
}

VkInstance CreateInstance(bool enableValidationLayers, const std::vector<const char*>& validationLayers)
{
	ASSERT(enableValidationLayers <= CheckValidationLayerSupport(validationLayers), "Validation layers requested but not available");

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "VolcanicSnake";
	appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	appInfo.pEngineName = "No engine";
	appInfo.engineVersion = NULL;
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instanceCreateInfo{};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
	{
		extensions.push_back("VK_EXT_debug_utils");
	}

	instanceCreateInfo.enabledExtensionCount = extensions.size();;
	instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

	if (enableValidationLayers)
	{
		instanceCreateInfo.enabledLayerCount = (uint32_t)(validationLayers.size());
		instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
		
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = GetDebugMessengerInfo();
		instanceCreateInfo.pNext = &debugCreateInfo;
	}
	else
	{
		instanceCreateInfo.enabledLayerCount = 0;
	}

	VkInstance instance;
	VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);

	ASSERT(result == VK_SUCCESS, "Failed to create the vulkan instance.");

	return instance;
}

bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers)
{
	uint32_t propertyCount;
	vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(propertyCount);
	vkEnumerateInstanceLayerProperties(&propertyCount, availableLayers.data());

	for (const char* requestedLayerName : validationLayers)
	{
		bool layerAvailable = false;

		for (const VkLayerProperties& properties : availableLayers)
		{
			if (strcmp(properties.layerName, requestedLayerName) == 0)
			{
				layerAvailable = true;
			}
		}

		if (!layerAvailable)
		{
			return false;
		}
	}

	return true;
}

VkDebugUtilsMessengerEXT SetupDebugMessenger(VkInstance instance)
{
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = GetDebugMessengerInfo();

	VkDebugUtilsMessengerEXT debugMessenger;
	VkResult result = CreateDebugUtilsMessengerEXT(instance, &debugCreateInfo, nullptr, &debugMessenger);

	ASSERT(result == VK_SUCCESS, "Failed to create the debug messenger");

	return debugMessenger;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (func == nullptr)
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

VkSurfaceKHR CreateSurface(VkInstance instance, GLFWwindow* window)
{
	VkSurfaceKHR surface;
	VkResult result = glfwCreateWindowSurface(instance, window, NULL, &surface);
	
	return surface;
}

VkPhysicalDevice ChoosePhysicalDevice(VkInstance instance, VkSurfaceKHR surface, const std::vector<const char*>& deviceExtensions)
{
	// Get number of devices first
	uint32_t physicalDeviceCount;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

	// Then create & fill the buffer
	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	QueueFamilyIndices queueFamilyIndices;

	for (const VkPhysicalDevice& device : physicalDevices)
	{
		queueFamilyIndices = GetQueueFamilies(device, surface);

		// Check if required extensions, present modes, surface formats... are supported
		if (queueFamilyIndices.IsComplete() && IsDeviceSuitable(device, surface, deviceExtensions))
		{
			physicalDevice = device;
			break;
		}
	}

	ASSERT(physicalDevice != VK_NULL_HANDLE, "Couldn't find a suitable GPU.");

	return physicalDevice;
}

QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	// Really good queue (family) explaination lol: https://stackoverflow.com/questions/55272626/what-is-actually-a-queue-family-in-vulkan

	QueueFamilyIndices indices;

	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	for (int i = 0; i < queueFamilies.size(); i++)
	{
		const VkQueueFamilyProperties& queueFamily = queueFamilies[i];

		if (queueFamily.queueCount <= 0)
		{
			continue;
		}

		// We need at least of queue family that can do graphic operations (amongst others)
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.GraphicsFamily = i;
			indices.HasGraphicsFamily = true;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

		if (presentSupport)
		{
			indices.PresentFamily = i;
			indices.HasPresentFamily = true;
		}

		if (indices.IsComplete())
		{
			break;
		}
	}

	return indices;
}

bool SupportsRequiredExtensions(const VkPhysicalDevice& device, const std::vector<const char*>& deviceExtensions)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	for (const VkExtensionProperties& extension : availableExtensions)
	{
		// TODO: check for all extensions...
		if (strcmp(extension.extensionName, deviceExtensions[0]) == 0)
		{
			return true;
		}
	}

	return false;
}

SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
{
	SwapChainSupportDetails details{};

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.Capabilities);

	// VK_FORMAT_R32G32B32A32_SFLOAT and VK_COLOR_SPACE_SRGB_NONLINEAR_KHR for example
	uint32_t surfaceFormatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &surfaceFormatCount, nullptr);

	if (surfaceFormatCount != 0)
	{
		details.SurfaceFormats.resize(surfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &surfaceFormatCount, details.SurfaceFormats.data());
	}

	// Mailbox, Fifo, ...
	uint32_t presentModesCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModesCount, nullptr);

	if (presentModesCount != 0)
	{
		details.PresentModes.resize(presentModesCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModesCount, details.PresentModes.data());
	}

	return details;
}

bool SupportsAnisotropicSampling(const VkPhysicalDevice& device)
{
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(device, &features);

	return features.samplerAnisotropy;
}

bool IsDeviceSuitable(const VkPhysicalDevice& device, const VkSurfaceKHR& surface, const std::vector<const char*>& deviceExtensions)
{
	SwapChainSupportDetails swapChainDetails = QuerySwapChainSupport(device, surface);
	return SupportsRequiredExtensions(device, deviceExtensions) && swapChainDetails.IsComplete() && SupportsAnisotropicSampling(device);
}

uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memoryProperties{};
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if ((typeFilter) & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	ASSERT(false, "Failed to find a supported memory type.");
}

VkDevice ChooseLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const QueueFamilyIndices& indices, const std::vector<const char*>& deviceExtensions)
{
	// Create 2 queues, one present queue and one graphics queue
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily, indices.PresentFamily };

	for (uint32_t queueFamilyIndex : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

		// Create one queue from the family ???
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;

		static constexpr float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	// TODO: only set ansitorepoa filtering to true
	VkPhysicalDeviceFeatures physicalDeviceFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeatures);
	deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

	deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

	deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

	// Debug layer ???
	deviceCreateInfo.enabledLayerCount = 0;
	// deviceCreateInfo.ppEnabledLayerNames = nullptr;

	VkDevice device;
	VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);

	ASSERT(result == VK_SUCCESS, "Failed to create the logical device.");
	
	return device;
}

VkQueue GetQueue(VkDevice device, uint32_t queueFamily, uint32_t queueIndex /* = 0 */)
{
	VkQueue queue;
	vkGetDeviceQueue(device, queueFamily, queueIndex, &queue);

	return queue;
}

VkCommandPool CreateCommandPool(VkDevice device, uint32_t queueFamilyIndex)
{
	VkCommandPoolCreateInfo poolCreateInfo{};
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

	// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkCommandPoolCreateFlagBits.html
	poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	poolCreateInfo.queueFamilyIndex = queueFamilyIndex;

	VkCommandPool commandPool;
	vkCreateCommandPool(device, &poolCreateInfo, nullptr, &commandPool);

	return commandPool;
}

VkSurfaceFormatKHR ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	// Choose SRGB if available, else just default to the first format in the vector

	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availableModes)
{
	// Choose Mailbox if available, else just default to Fifo

	for (const auto& presentMode : availableModes)
	{
		if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return presentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
	if (surfaceCapabilities.currentExtent.height != UINT32_MAX)
	{
		return surfaceCapabilities.currentExtent;
	}
	
#ifndef PLATFORM_WINDOWS
	ASSERT(false, "Not yet implemented.");
#endif

	return { WIDTH, HEIGHT };
}

VkSwapchainKHR CreateSwapChain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR capabilities, VkExtent2D extent, VkSurfaceFormatKHR surfaceFormat, VkPresentModeKHR presentMode, const QueueFamilyIndices& queueIndices)
{
	uint32_t imageCount = capabilities.minImageCount + 1;

	// == 0 -> no limit
	uint32_t maxImageCount = capabilities.maxImageCount;
	if (maxImageCount != 0 && imageCount > maxImageCount)
	{
		imageCount = maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

	createInfo.imageExtent = extent;
	createInfo.surface = surface;
	createInfo.presentMode = presentMode;

	createInfo.minImageCount = imageCount;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	createInfo.imageFormat = surfaceFormat.format;

	// Would be 6 for cubemap (I think???)
	createInfo.imageArrayLayers = 1;

	uint32_t queueFamilyIndices[2] = { queueIndices.GraphicsFamily, queueIndices.PresentFamily };

	if (queueIndices.GraphicsFamily == queueIndices.PresentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}

	// Depending on the physical device, Vulkan might apply a rotation/mirror transform
	createInfo.preTransform = capabilities.currentTransform;

	// No blending
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	// Side effect: The image might not own all of its pixels, f.e. if the window is obscured by another window
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VkSwapchainKHR swapChain;
	VkResult result = vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain);

	ASSERT(result == VK_SUCCESS, "Failed to create the swap chain.");

	return swapChain;
}

void GetSwapChainImages(VkDevice device, VkSwapchainKHR swapChain, std::vector<VkImage>& images)
{
	// We've only specified a minimum number of swap chain images, the implementation is allowed to create more

	uint32_t imageCount;
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);

	images.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, images.data());
}

void CreateImageViews(VkDevice device, const std::vector<VkImage>& swapChainImages, VkFormat imageFormat, std::vector<VkImageView>& imageViews)
{
	uint32_t imageCount = swapChainImages.size();
	imageViews.resize(imageCount);

	for (uint32_t i = 0; i < imageCount; i++)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

		viewInfo.image = swapChainImages[i];
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = imageFormat;

		// Which part of the image should be accessed (multiple layers, color or depth, ...)?
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;

		VkResult result = vkCreateImageView(device, &viewInfo, nullptr, &imageViews[i]);
		
		ASSERT(result == VK_SUCCESS, "Failed to create a swap chain image view.");
	}
}

VkFormat FindSupportedDepthFormat(VkPhysicalDevice physicalDevice)
{
	// VK_FORMAT_D32_SFLOAT ... 32 bit for depth
	// VK_FORMAT_D32_SFLOAT_S8_UINT ... 32 bit for depth, 8 bit for stencil
	// VK_FORMAT_D24_UNORM_S8_UINT ... 24 bit for depth, 8 bit for stencil
	VkFormat depthFormats[3] = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };

	// VK_IMAGE_TILING_LINEAR ... texels are laid out as row-major
	// VK_IMAGE_TILING_OPTIMAL ... implementation dependent, used most optimal layout
	VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;

	// Needs to be set so that image can be used as a depth/stencil attachment
	VkFormatFeatureFlags formatFeatures = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

	for (auto format : depthFormats)
	{
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);

		if (formatProperties.optimalTilingFeatures & formatFeatures)
		{
			return format;
		}
	}

	ASSERT(false, "Failed to find an appropiate depth format.");
}

void CreateDepthResources(VkDevice device, VkPhysicalDevice physicalDevice, VkMemoryPropertyFlags properties, VkFormat depthFormat, VkExtent2D swapChainExtent, uint32_t imageCount, std::vector<VkImage>& depthImages, std::vector<VkDeviceMemory>& depthMemory, std::vector<VkImageView>& depthImageViews)
{
	depthImages.resize(imageCount);
	depthMemory.resize(imageCount);
	depthImageViews.resize(imageCount);

	for (uint32_t i = 0; i < imageCount; i++)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;

		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.format = depthFormat;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		imageInfo.extent.width = swapChainExtent.width;
		imageInfo.extent.height = swapChainExtent.height;
		imageInfo.extent.depth = 1;

		imageInfo.arrayLayers = 1;
		imageInfo.mipLevels = 1;

		VkResult result = vkCreateImage(device, &imageInfo, nullptr, &depthImages[i]);

		ASSERT(result == VK_SUCCESS, "Failed to create a depth image.");

		VkMemoryRequirements memoryRequirements{};
		vkGetImageMemoryRequirements(device, depthImages[i], &memoryRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, properties);

		result = vkAllocateMemory(device, &allocInfo, nullptr, &depthMemory[i]);
		ASSERT(result == VK_SUCCESS, "Failed to allocate memory for the image.");

		result = vkBindImageMemory(device, depthImages[i], depthMemory[i], 0);
		ASSERT(result == VK_SUCCESS, "Failed to bind the image memory.");

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

		viewInfo.image = depthImages[i];
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = depthFormat;

		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		result = vkCreateImageView(device, &viewInfo, nullptr, &depthImageViews[i]);
		ASSERT(result == VK_SUCCESS, "Failed to create a depth image view.");
	}
}

VkRenderPass CreateRenderPass(VkDevice device, VkPhysicalDevice physicalDevice, VkFormat imageFormat, VkFormat depthFormat)
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = imageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	// VK_IMAGE_LAYOUT_PRESENT_SRC_KHR can only be used for presenting an image to the screen
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorReference{};
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = depthFormat;

	// One sample per pixel
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

	// Clear depth attachment at beginning of subpass
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;

	// Contents of the buffer may be discareded after rendering to it
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthReference{};
	depthReference.attachment = 1;
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription{};

	// The pipeline bound to VK_PIPELINE_BIND_POINT_GRAPHICS controls the behavior of all drawing commands
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorReference;
	subpassDescription.pDepthStencilAttachment = &depthReference;

	// Awesome explanation: https://www.reddit.com/r/vulkan/comments/s80reu/comment/hth2uj9/?utm_source=share&utm_medium=web2x&context=3
	// https://stackoverflow.com/questions/53984863/what-exactly-is-vk-subpass-external
	VkSubpassDependency subpassDependency{};

	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependency.dstSubpass = 0;

	subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	subpassDependency.srcAccessMask = NULL;

	subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

	renderPassInfo.attachmentCount = attachments.size();
	renderPassInfo.pAttachments = attachments.data();

	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &subpassDependency;

	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpassDescription;

	VkRenderPass renderPass;
	VkResult result = vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass);

	ASSERT(result == VK_SUCCESS, "Failed to create the render pass.");

	return renderPass;
}

void CreateFramebuffers(VkDevice device, VkRenderPass renderPass, VkExtent2D swapChainExtent, uint32_t imageCount, const std::vector<VkImageView>& colorImageViews, const std::vector<VkImageView>& depthImageViews, std::vector<VkFramebuffer>& framebuffers)
{
	framebuffers.resize(imageCount);

	for (uint32_t i = 0; i < imageCount; i++)
	{
		std::array<VkImageView, 2> attachments = { colorImageViews[i], depthImageViews[i] };

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();

		framebufferInfo.renderPass = renderPass;

		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		VkResult result = vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffers[i]);
		ASSERT(result == VK_SUCCESS, "Failed to create a framebuffer.");
	}
}

void CreateSyncObjects(VkDevice device, uint32_t imageCount, SyncObjects& syncObjects)
{
	syncObjects.ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	syncObjects.RenderingFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	syncObjects.InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	syncObjects.ImagesInFlight.resize(imageCount, VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

	// VK_FENCE_CREATE_SIGNALED_BIT specifies that the fence object is created in the signaled state. Otherwise, it is created in the unsignaled state.
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		VkResult result = (VkResult)(
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &syncObjects.ImageAvailableSemaphores[i]) +
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &syncObjects.RenderingFinishedSemaphores[i]) +
			vkCreateFence(device, &fenceInfo, nullptr, &syncObjects.InFlightFences[i])
		);

		ASSERT(result == VK_SUCCESS, "Failed to create the synchronization objects.");
	}
}

VkPipelineLayout CreatePipelineLayout(VkDevice device)
{
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(ShaderMatrices);

	VkPipelineLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	layoutInfo.setLayoutCount = 0;
	layoutInfo.pSetLayouts = nullptr;
	layoutInfo.pushConstantRangeCount = 1;
	layoutInfo.pPushConstantRanges = &pushConstantRange;

	VkPipelineLayout pipelineLayout;
	VkResult result = vkCreatePipelineLayout(device, &layoutInfo, nullptr, &pipelineLayout);

	ASSERT(result == VK_SUCCESS, "Failed to create the pipeline layout.");

	return pipelineLayout;
}

VkPipeline CreatePipeline(VkDevice device, VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const fs::path& vertexShaderPath, const fs::path& fragmentShaderPath)
{
	// The render pass would usually be member of the custom swap chain class
	// ASSERT(swapChain != VK_NULL_HANDLE, "You need a valid swap chain before creating a pipeline.");

	ASSERT(renderPass != VK_NULL_HANDLE, "You need a valid render pass before creating a pipeline.");
	ASSERT(pipelineLayout != VK_NULL_HANDLE, "You need a valid pipeline layout when creating a pipeline.");

	PipelineConfig config{};
	config.UseDefaults();

	config.RenderPass = renderPass;
	config.PipelineLayout = pipelineLayout;

	std::string vertexSource = ReadFile(vertexShaderPath);
	std::string fragmentSource = ReadFile(fragmentShaderPath);

	VkShaderModule vertexModule = CreateShaderModule(device, vertexSource);
	VkShaderModule fragmentModule = CreateShaderModule(device, fragmentSource);

	VkPipelineShaderStageCreateInfo shaderStageInfos[2]{};

	for (int i = 0; i < 2; i++)
	{
		shaderStageInfos[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfos[i].pName = "main";
	}

	shaderStageInfos[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStageInfos[0].module = vertexModule;

	shaderStageInfos[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStageInfos[1].module = fragmentModule;

	std::vector<VkVertexInputBindingDescription> bindingDescriptions = Vertex::GetBindingDescriptions();
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions = Vertex::GetAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexStateInfo{};
	vertexStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	vertexStateInfo.vertexBindingDescriptionCount = bindingDescriptions.size();
	vertexStateInfo.pVertexBindingDescriptions = bindingDescriptions.data();

	vertexStateInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
	vertexStateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStageInfos;
	pipelineInfo.pVertexInputState = &vertexStateInfo;

	pipelineInfo.pViewportState = &config.ViewportInfo;
	pipelineInfo.pInputAssemblyState = &config.InputAssemblyInfo;
	pipelineInfo.pRasterizationState = &config.RasterizationInfo;
	pipelineInfo.pMultisampleState = &config.MultisampleInfo;
	pipelineInfo.pColorBlendState = &config.ColorBlendInfo;
	pipelineInfo.pDepthStencilState = &config.DepthStencilInfo;
	pipelineInfo.pDynamicState = &config.DynamicStateInfo;

	// These don't have default values!
	pipelineInfo.layout = config.PipelineLayout;
	pipelineInfo.renderPass = config.RenderPass;
	pipelineInfo.subpass = config.Subpass;

	// Optional performance optimizations
	pipelineInfo.basePipelineIndex = -1;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	VkPipeline pipeline;
	VkResult result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);

	ASSERT(result == VK_SUCCESS, "Failed to create the pipeline.");

	return pipeline;
}

void PipelineConfig::UseDefaults()
{
	{
		ViewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

		ViewportInfo.viewportCount = 1;
		ViewportInfo.pViewports = nullptr;

		ViewportInfo.scissorCount = 1;
		ViewportInfo.pScissors = nullptr;
	}

	{
		InputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

		// Vertices: a, b, c, d, e, f
		// Triangle list: triangles (a, b), (b, c), (e, f)
		// Triangle strip: triangles (a, b), (b, c), (c, d), (d, e), (e, f)
		// Could alse be point/list/...
		InputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
	}

	{
		RasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

		// If enabled: clamp(gl_Position.z, 0.0, 1.0)
		// Doesn't make sense for most applications because if the z value is < 0, it's behind the camera
		// > 1: Vertex is outside of the view frustum
		RasterizationInfo.depthClampEnable = VK_FALSE;

		// Discard all fragments immediately
		RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;

		RasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		RasterizationInfo.lineWidth = 1.0f;
		RasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		RasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

		// The depth values of all fragments are generated by the rasterization of a polygon
		// Can be offset by a single value that is computed for that polygon
		RasterizationInfo.depthBiasEnable = VK_FALSE;
		RasterizationInfo.depthBiasConstantFactor = 0.0f;
		RasterizationInfo.depthBiasClamp = 0.0f;
		RasterizationInfo.depthBiasSlopeFactor = 0.0f;
	}

	{
		// Multisampling is used to prevent aliasing
		// No multisampling: if fragments's center is contained in triangle: fragment is "fully" in triangle
		// Results in jagged lines
		// Multisampling: Multiple samples (VK_SAMPLE_COUNT_4_BIT or 8_BIT etc.); determines how much of a pixel
		// contained in the triangle
		// MSAA ... Multisample Anti Aliasing
		MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		MultisampleInfo.sampleShadingEnable = VK_FALSE;
		MultisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		MultisampleInfo.minSampleShading = 1.0f;
		MultisampleInfo.pSampleMask = nullptr;
		MultisampleInfo.alphaToCoverageEnable = VK_FALSE;
		MultisampleInfo.alphaToOneEnable = VK_FALSE;
	}

	{
		ColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		ColorBlendAttachment.blendEnable = VK_FALSE;

		ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		ColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;

		ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		ColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
	}

	{
		ColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

		// Logical op: &, |, ^, ...
		// Copy(a, b) = a
		// Is performed on all color values before writing them into the framebuffer
		// Only for signed-, unsigned- and normalized integer framebuffers
		// Not for floating point or SRGB framebuffers
		ColorBlendInfo.logicOpEnable = VK_FALSE;
		ColorBlendInfo.logicOp = VK_LOGIC_OP_COPY;

		ColorBlendInfo.attachmentCount = 1;
		ColorBlendInfo.pAttachments = &ColorBlendAttachment;
		ColorBlendInfo.blendConstants[0] = 0.0f;
		ColorBlendInfo.blendConstants[1] = 0.0f;
		ColorBlendInfo.blendConstants[2] = 0.0f;
		ColorBlendInfo.blendConstants[3] = 0.0f;
	}

	{
		DepthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		DepthStencilInfo.depthTestEnable = VK_TRUE;
		DepthStencilInfo.depthWriteEnable = VK_TRUE;
		DepthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;

		// Depth Bounds Test: discard fragments if their depth isn't in the specified range			
		DepthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		DepthStencilInfo.minDepthBounds = 0.0f;
		DepthStencilInfo.maxDepthBounds = 1.0f;

		DepthStencilInfo.stencilTestEnable = VK_FALSE;
		DepthStencilInfo.front = {};
		DepthStencilInfo.back = {};
	}

	{
		DynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		DynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		DynamicStateInfo.pDynamicStates = DynamicStateEnables.data();
		DynamicStateInfo.dynamicStateCount = DynamicStateEnables.size();
		DynamicStateInfo.flags = NULL;
	}
}

std::string ReadFile(const fs::path& filePath)
{
	// ate ... put cursor at the end
	std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	ASSERT(file.is_open(), "Failed to open file \"" + filePath.string() + "\"");

	size_t size = (size_t)file.tellg();

	std::string content(size, ' ');
	file.seekg(0);
	file.read(&content[0], size);

	return content;
}

VkShaderModule CreateShaderModule(VkDevice device, const std::string& shaderSource)
{
	VkShaderModuleCreateInfo moduleInfo{};
	moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

	moduleInfo.codeSize = shaderSource.size();
	moduleInfo.pCode = (const uint32_t*)shaderSource.c_str();

	VkShaderModule module;
	VkResult result = vkCreateShaderModule(device, &moduleInfo, nullptr, &module);

	ASSERT(result == VK_SUCCESS, "Failed to create a shader module.");

	return module;
}

VkBuffer CreateVertexBuffers(VkDevice device, VkPhysicalDevice physicalDevice, const std::vector<Vertex>& vertices, VkDeviceMemory& deviceMemory)
{
	uint32_t vertexCount = vertices.size();
	ASSERT(vertexCount >= 3, "There must be more than 3 vertices.");

	VkDeviceSize bufferSize = vertexCount * sizeof(Vertex);

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

	bufferInfo.size = bufferSize;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkBuffer vertexBuffer;
	VkResult result = vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer);

	ASSERT(result == VK_SUCCESS, "Failed to create a vertex buffer.");

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(device, vertexBuffer, &memoryRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

	allocInfo.allocationSize = memoryRequirements.size;

	// HOST_VISIBLE_BIT: allocated memory is accessible from host
	// HOST_COHERENT_BIT: once the host memory is changed, it automatically will be flushed to update the device memory
	// without this option, this would have to be done manually vkFlushMappedMemoryRanges
	allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	result = vkAllocateMemory(device, &allocInfo, nullptr, &deviceMemory);

	ASSERT(result == VK_SUCCESS, "Failed to allocate vertex buffer memory.");

	vkBindBufferMemory(device, vertexBuffer, deviceMemory, 0);

	void* data;
	vkMapMemory(device, deviceMemory, 0, bufferSize, NULL, &data);
	memcpy(data, vertices.data(), bufferSize);
	vkUnmapMemory(device, deviceMemory);

	return vertexBuffer;
}

void CreateCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t imageCount, std::vector<VkCommandBuffer>& commandBuffers)
{
	commandBuffers.resize(imageCount);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

	allocInfo.commandBufferCount = commandBuffers.size();

	// All of the command buffers will be created in the command pool
	// This avoids having to search for new, free memory when allocating a new command buffer
	allocInfo.commandPool = commandPool;

	// Primary command buffers: can be queued for execution, but can't be called by other command buffers
	// Secondary command buffers: can be called by other command buffers, but be can't queued for execution
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	VkResult result = vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data());

	ASSERT(result == VK_SUCCESS, "Failed to allocate the command buffers.");
}

void RecordCommandBuffer(const std::array<glm::mat4, 3> transforms, VkCommandBuffer commandBuffer, VkFramebuffer framebuffer, VkExtent2D swapChainExtent, VkRenderPass renderPass, VkPipeline pipeline, VkPipelineLayout layout, VkBuffer vertexBuffer, uint32_t vertexCount)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
	ASSERT(result == VK_SUCCESS, "Failed to start recording a command buffer.");

	std::array<VkClearValue, 2> clearValues = { {
		{ 0.01, 0.01f, 0.01f, 1.0f },
		{ 1.0f, 0 }
	} };

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

	renderPassInfo.framebuffer = framebuffer;
	renderPassInfo.renderPass = renderPass;

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;

	renderPassInfo.clearValueCount = clearValues.size();
	renderPassInfo.pClearValues = clearValues.data();

	// Subpass: use secondary command buffers?
	// VK_SUBPASS_CONTENTS_INLINE: nope
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};

	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = swapChainExtent.width;
	viewport.height = swapChainExtent.height;

	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};

	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;

	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	// VK_PIPELINE_BIND_POINT_GRAPHICS ... graphics pipeline
	// VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR ... raytracing pipeline
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

	VkBuffer buffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

	static glm::mat4 projection = glm::ortho(-ASPECT_RATIO, ASPECT_RATIO, -1.0f, 1.0f);

	static ShaderMatrices matrices;
	matrices.Projection = projection;

	for (const auto& transform : transforms)
	{
		matrices.Transform = transform;

		vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(ShaderMatrices), &matrices);
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	vkCmdEndRenderPass(commandBuffer);

	result = vkEndCommandBuffer(commandBuffer);
	ASSERT(result == VK_SUCCESS, "Failed to record a command buffer.");
}

void DrawFrame(VkDevice device, VkSwapchainKHR swapChain, VkQueue graphicsQueue, VkQueue presentQueue, const std::vector<VkCommandBuffer>& commandBuffers, SyncObjects& syncObjects, const std::array<glm::mat4, 3>& transforms, const std::vector<VkFramebuffer>& framebuffers, VkExtent2D swapChainExtent, VkRenderPass renderPass, VkPipeline pipeline, VkPipelineLayout pipelineLayout, VkBuffer vertexBuffer, uint32_t verticesSize)
{
	static uint32_t currentFrame = 0;

	uint32_t imageIndex = AquireNextImage(device, swapChain, syncObjects, currentFrame);

	RecordCommandBuffer(transforms, commandBuffers[imageIndex], framebuffers[imageIndex], swapChainExtent, renderPass, pipeline, pipelineLayout, vertexBuffer, verticesSize);
	SubmitCommandBuffers(device, swapChain, graphicsQueue, presentQueue, commandBuffers[imageIndex], syncObjects, imageIndex, currentFrame);

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

uint32_t AquireNextImage(VkDevice device, VkSwapchainKHR swapChain, SyncObjects& syncObjects, uint32_t currentFrame)
{
	vkWaitForFences(device, 1, &syncObjects.InFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, syncObjects.ImageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	ASSERT(result == VK_SUCCESS, "Failed to aquire a swap chain image.");

	return imageIndex;
}

void SubmitCommandBuffers(VkDevice device, VkSwapchainKHR swapChain, VkQueue graphicsQueue, VkQueue presentQueue, VkCommandBuffer commandBuffer, SyncObjects& syncObjects, uint32_t imageIndex, uint32_t currentFrame)
{
	// Wait for the current frame we're rendering to to be signaled
	if (syncObjects.ImagesInFlight[currentFrame] != VK_NULL_HANDLE)
	{
		vkWaitForFences(device, 1, &syncObjects.ImagesInFlight[currentFrame], VK_TRUE, UINT64_MAX);
	}

	syncObjects.ImagesInFlight[imageIndex] = syncObjects.InFlightFences[currentFrame];
	vkResetFences(device, 1, &syncObjects.InFlightFences[currentFrame]);

	VkPipelineStageFlags waitMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &syncObjects.ImageAvailableSemaphores[currentFrame];
	submitInfo.pWaitDstStageMask = &waitMask;

	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &syncObjects.RenderingFinishedSemaphores[currentFrame];

	// The InFlightFence will be signaled after all command buffers have completed execution
	VkResult result = vkQueueSubmit(graphicsQueue, 1, &submitInfo, syncObjects.InFlightFences[currentFrame]);
	ASSERT(result == VK_SUCCESS, "Failed to submit a command buffer to the queue.");

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapChain;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &syncObjects.RenderingFinishedSemaphores[currentFrame];

	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(presentQueue, &presentInfo);
	// ASSERT(result == VK_SUCCESS, "Failed to present swap chain image"); ???
}

std::array<glm::mat4, 3> CalculateTransforms(const std::array<glm::vec2, 3>& positions)
{
	std::array<glm::mat4, 3> transforms = {
		glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(positions[0], 0.0f)), { PLAYER_WIDTH, PLAYER_HEIGHT, 1.0f }),
		glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(positions[1], 0.0f)), { PLAYER_WIDTH, PLAYER_HEIGHT, 1.0f }),
		glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(positions[2], 0.0f)), { BALL_SIZE, BALL_SIZE, 1.0f })
	};

	return transforms;
}

void MovePlayer(float& position, float amount)
{
	float newPosition = position + amount;

	// Collision detection

	for (int i = 0; i < 2; i++)
	{
		int sign = 2 * i - 1;
		float bound = newPosition + sign * (PLAYER_HEIGHT / 2.0f + PADDING);

		if (abs(bound) >= 1.0f)
		{
			return;
		}
	}

	position = newPosition;
}

int MoveBall(std::array<glm::vec2, 3>& positions, glm::vec2& direction)
{
	float angle = abs(glm::dot(direction, glm::vec2(0.0f, 1.0f)));
	float ballSpeed = MAX_BALL_SPEED * angle / glm::pi<float>();
	ballSpeed = std::max(MIN_BALL_SPEED, ballSpeed);

	glm::vec2& ballPosition = positions[2];
	glm::vec2 newPosition = ballPosition + direction * ballSpeed;

	// Check for ceiling/floor collision

	for (int i = 0; i < 2; i++)
	{
		int sign = 2 * i - 1;
		float bound = newPosition.y + sign * (BALL_SIZE / 2.0f);

		if (abs(bound) >= 1.0f)
		{
			Bounce({ 0.0f, 1.0f }, direction);
			return 0;
		}
	}

	// Check for player/goal collision

	for (int i = 0; i < 2; i++)
	{
		int sign = 2 * i - 1;
		float bound = newPosition.x + sign * (BALL_SIZE / 2.0f);

		if (abs(bound) < ASPECT_RATIO - PLAYER_POSITION)
		{
			continue;
		}

		// Player collision

		bool didCollide = false;
		int player = (glm::sign(direction.x) + 1) / 2;

		if (newPosition.y - BALL_SIZE > (positions[player].y + PLAYER_HEIGHT / 2.0f) || newPosition.y + BALL_SIZE < (positions[player].y - PLAYER_HEIGHT / 2.0f))
		{
			ballPosition = { 0.0f, 0.0f };
			direction = glm::normalize(glm::vec2(1.0f - player * 2.0f, 1.0f));

			return 2 - player;
		}

		Bounce({ 1.0f, 0.0f }, direction);
		return 0;
	}
	
	ballPosition = newPosition;

	return 0;
}

void Bounce(const glm::vec2& surfaceNormal, glm::vec2& direction)
{
	float random = rand() / (float)RAND_MAX;
	random = random * 0.1f - 0.05f;

	direction = glm::reflect(direction, surfaceNormal + glm::vec2(random, 0.0f));

	int sign = glm::sign(direction.x);
	direction.x = sign * std::max(0.5f, abs(direction.x));
}