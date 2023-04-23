// IFT3100H23_PBR/renderer.cpp
// Classe responsable du rendu de l'application.

#include "pbr.h"

void Pbr::setup()
{
    ofSetFrameRate(60);
    ofSetBackgroundColor(0);
    ofDisableArbTex();
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetSphereResolution(32);

    // chargement d'un mod�le 3D
    teapot.loadModel("geometry/model_fille.obj");
    

    // d�sactiver le mat�riau par d�faut du mod�le
    teapot.disableMaterials();

    // ofVec3f modelPosition(ofGetWidth() / 2, ofGetHeight() / 2, 0);
     //teapot.setPosition(ofGetWidth() / 2, ofGetHeight(), 0);
     // charger, compiler et linker les sources des shaders
    shader.load(
        "shader/pbr_330_vs.glsl",
        "shader/pbr_330_fs.glsl");

    // charger les textures du mat�riau
    texture_diffuse.load("texture/ambiante.jpg");
    texture_metallic.load("texture/metallique.jpg");
    texture_roughness.load("texture/metal_plate_roughness_1k.jpg");
    texture_occlusion.load("texture/metal_plate_ao_1k.jpg");

    // param�tres des textures du mat�riau
    texture_diffuse.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    texture_metallic.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    texture_roughness.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    texture_occlusion.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
}

void Pbr::reset()
{
    // param�tres de transformation
    speed_motion = 150.0f;
    speed_rotation = 50.0f;
    initial_x = 0.0f;
    initial_z = -100.0f;
    rotation_y = 0.0f;
    scale_cube = 110.0f;
    scale_sphere = 90.0f;
    scale_teapot = 0.618f;

    // positions initiales des maillages g�om�triques
    position_cube.set(-ofGetWidth() * (1.0f / 4.0f), 0.0f, 0.0f);
    position_teapot.set(0.0f, 0.0f, 0.0f);
    position_teapot.set(ofGetWidth() * (1.0f / 4.0f), 50.0f, 0.0f);

    // param�tres du mat�riau
    material_color_ambient = ofColor(63, 63, 63);
    material_color_diffuse = ofColor(255, 255, 255);
    material_color_specular = ofColor(255, 255, 255);

    material_metallic = 0.5f;
    material_roughness = 0.5f;
    material_occlusion = 1.0f;
    material_brightness = 1.0f;

    material_fresnel_ior = glm::vec3(0.04f, 0.04f, 0.04f);

    // param�tres de la lumi�re
    light_color = ofColor(255, 255, 255);
    light_intensity = 1.0f;
    light_motion = true;

    // param�tres de mappage tonal
    tone_mapping_exposure = 1.0f;
    tone_mapping_toggle = true;

    // initialisation des variables
    delta_x = speed_motion;
    delta_y = speed_rotation;
    delta_z = speed_motion;

    // position initiale de la cam�ra
    offset_x = initial_x;
    offset_z = initial_z;

    ofLog() << "<reset>";
}

void Pbr::update()
{
    // centre de la fen�tre d'affichage
    center_x = ofGetWidth() / 2.0f;
    center_y = ofGetHeight() / 2.0f;

    if (light_motion)
    {
        // transformer la lumi�re
        light.setGlobalPosition(
            ofMap(ofGetMouseX() / (float)ofGetWidth(), 0.0f, 1.0f, -center_x, center_x),
            ofMap(ofGetMouseY() / (float)ofGetHeight(), 0.0f, 1.0f, -center_y, center_y),
            -offset_z * 1.0f);
    }
    teapot.setPosition(ofGetWidth() / 2, ofGetHeight() / 2, 0);

    // passer les attributs uniformes au shader de sommets
    shader.begin();

    shader.setUniform3f("material_color_ambient", material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
    shader.setUniform3f("material_color_diffuse", material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
    shader.setUniform3f("material_color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);

    shader.setUniform1f("material_brightness", material_brightness);
    shader.setUniform1f("material_metallic", material_metallic);
    shader.setUniform1f("material_roughness", material_roughness);
    shader.setUniform1f("material_occlusion", material_occlusion);

    shader.setUniform3f("material_fresnel_ior", material_fresnel_ior);

    shader.setUniformTexture("texture_diffuse", texture_diffuse.getTexture(), 1);

    shader.setUniformTexture("texture_metallic", texture_metallic.getTexture(), 2);
    shader.setUniformTexture("texture_roughness", texture_roughness.getTexture(), 3);
    shader.setUniformTexture("texture_occlusion", texture_occlusion.getTexture(), 4);

    shader.setUniform1f("light_intensity", light_intensity);
    shader.setUniform3f("light_color", light_color.r / 255.0f, light_color.g / 255.0f, light_color.b / 255.0f);
    shader.setUniform3f("light_position", light.getGlobalPosition());

    shader.setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
    shader.setUniform1f("tone_mapping_gamma", tone_mapping_gamma);
    shader.setUniform1i("tone_mapping_toggle", tone_mapping_toggle);

    shader.end();
}

void Pbr::draw()
{

    
   // ofSetFullscreen(true);
    // activer l'occlusion en profondeur
    ofEnableDepthTest();

    // activer l'�clairage dynamique
    ofEnableLighting();

    // activer la lumi�re dynamique
    light.enable();

    // activer le shader
    shader.begin();

    ofPushMatrix();

    // transformer l'origine de la sc�ne au milieu de la fen�tre d'affichage
    ofTranslate(center_x + offset_x, center_y, offset_z);
    

    ofPopMatrix();

    ofPushMatrix();
   
// positionner le teapot
    teapot.setPosition(
        ofGetWidth() / 2,
        ofGetHeight()/2.5,
        position_teapot.z);
    
    ofRotateYDeg(45.0f);
    ofRotateXDeg(45);
    ofRotateZDeg(45);
    ofPushMatrix();
    
    teapot.draw(OF_MESH_FILL);
    
    ofPopMatrix();

    ofPopMatrix();

    
    // d�sactiver le shader
    shader.end();

    // d�sactiver la lumi�re
    light.disable();
    
    // d�sactiver l'�clairage dynamique
    ofDisableLighting();

    // d�sactiver l'occlusion en profondeur
    ofDisableDepthTest();
}
