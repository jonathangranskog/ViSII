#include <visii/light.h>

Light Light::lights[MAX_LIGHTS];
LightStruct Light::lightStructs[MAX_LIGHTS];
std::map<std::string, uint32_t> Light::lookupTable;
std::shared_ptr<std::mutex> Light::creationMutex;
bool Light::factoryInitialized = false;
bool Light::anyDirty = true;

Light::Light()
{
    this->initialized = false;
}

Light::Light(std::string name, uint32_t id)
{
    this->initialized = true;
    this->name = name;
    this->id = id;
    
    this->lightStructs[id].r = 1.f;
    this->lightStructs[id].g = 1.f;
    this->lightStructs[id].b = 1.f;
    this->lightStructs[id].intensity = 1.0;
    this->lightStructs[id].flags = 0;
}

std::string Light::toString() {
    std::string output;
    output += "{\n";
    output += "\ttype: \"Light\",\n";
    output += "\tname: \"" + name + "\"\n";
    output += "}";
    return output;
}

void Light::setColor(float r, float g, float b)
{
    lightStructs[id].r = max(0.f, min(r, 1.f));
    lightStructs[id].g = max(0.f, min(g, 1.f));
    lightStructs[id].b = max(0.f, min(b, 1.f));
    markDirty();
}

void Light::setColor(glm::vec3 color)
{
    lightStructs[id].r = max(0.f, min(color.r, 1.f));
    lightStructs[id].g = max(0.f, min(color.g, 1.f));
    lightStructs[id].b = max(0.f, min(color.b, 1.f));
    markDirty();
}

void Light::setTemperature(float kelvin)
{
    kelvin = max(kelvin, 0.f);

    float temp = kelvin / 100.0f;

    float red, green, blue;

    if ( temp <= 66 ){ 
        red = 255;         
        green = temp;
        green = 99.4708025861f * logf(green) - 161.1195681661f;

        if( temp <= 19){
            blue = 0;
        } else {
            blue = temp-10.f;
            blue = 138.5177312231f * logf(blue) - 305.0447927307f;
        }
    } else {
        red = temp - 60.f;
        red = 329.698727446f * powf(red, -0.1332047592f);
        
        green = temp - 60;
        green = 288.1221695283f * powf(green, -0.0755148492f );

        blue = 255;
    }

    lightStructs[id].r = red / 255.f;
    lightStructs[id].g = green / 255.f;
    lightStructs[id].b = blue / 255.f;
    markDirty();
}

void Light::setIntensity(float intensity)
{
    lightStructs[id].intensity = intensity;
    markDirty();
}

/* SSBO logic */
void Light::initializeFactory()
{
    if (isFactoryInitialized()) return;
    creationMutex = std::make_shared<std::mutex>();
    factoryInitialized = true;
}

bool Light::isFactoryInitialized()
{
    return factoryInitialized;
}

bool Light::areAnyDirty()
{
	return anyDirty;
}

void Light::markDirty() {
	dirty = true;
    anyDirty = true;
};

void Light::updateComponents()
{
	if (!anyDirty) return;

	for (int i = 0; i < MAX_LIGHTS; ++i) {
		if (lights[i].isDirty()) {
            lights[i].markClean();
        }
	};
	anyDirty = false;
} 

void Light::cleanUp()
{
    if (!isFactoryInitialized()) return;

    for (auto &light : lights) {
		if (light.initialized) {
			Light::remove(light.id);
		}
	}

    factoryInitialized = false;
}	

/* Static Factory Implementations */
Light* Light::create(std::string name) {
    auto l = StaticFactory::create(creationMutex, name, "Light", lookupTable, lights, MAX_LIGHTS);
    anyDirty = true;
    return l;
}

Light* Light::createFromTemperature(std::string name, float kelvin, float intensity) {
    auto light = StaticFactory::create(creationMutex, name, "Light", lookupTable, lights, MAX_LIGHTS);
    light->setTemperature(kelvin);
    light->setIntensity(intensity);
    return light;
}

Light* Light::createFromRGB(std::string name, glm::vec3 color, float intensity) {
    auto light = StaticFactory::create(creationMutex, name, "Light", lookupTable, lights, MAX_LIGHTS);
    light->setColor(color);
    light->setIntensity(intensity);
    return light;
}

Light* Light::get(std::string name) {
    return StaticFactory::get(creationMutex, name, "Light", lookupTable, lights, MAX_LIGHTS);
}

Light* Light::get(uint32_t id) {
    return StaticFactory::get(creationMutex, id, "Light", lookupTable, lights, MAX_LIGHTS);
}

void Light::remove(std::string name) {
    StaticFactory::remove(creationMutex, name, "Light", lookupTable, lights, MAX_LIGHTS);
    anyDirty = true;
}

void Light::remove(uint32_t id) {
    StaticFactory::remove(creationMutex, id, "Light", lookupTable, lights, MAX_LIGHTS);
    anyDirty = true;
}

Light* Light::getFront() {
    return lights;
}

LightStruct* Light::getFrontStruct() {
    return lightStructs;
}

uint32_t Light::getCount() {
    return MAX_LIGHTS;
}