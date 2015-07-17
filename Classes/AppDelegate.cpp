#include "AppDelegate.h"
#include "MainScene.h"

USING_NS_CC;



typedef enum{
    kRotateLandscape,
    kRotatePortrait,
}RotateType;

/**
 * every device resolution be control standard of iphone size.
 */



void settingResoulution( RotateType rtype ){
    // set platform
    Director *pDirector = CCDirector::getInstance();
    
    GLView* glView = pDirector->getOpenGLView();
    
    Size f_size = glView->getFrameSize();
    
    Size d_size = Size::ZERO;
    
    Application::Platform platform = Application::getInstance()->getTargetPlatform();
    
    CCLOG("%d", platform );
    
    
    
    
    int factor = 1;
    
    float rate = 1;
    
    float wideValue;
    
    if( rtype == kRotateLandscape ){
        if( f_size.width < f_size.height )
        {
            glView->setFrameSize( f_size.height , f_size.width );
            f_size = glView->getFrameSize();
        }
    }else if( rtype == kRotatePortrait ){
        if( f_size.width > f_size.height )
        {
            glView->setFrameSize( f_size.height , f_size.width );
            f_size = glView->getFrameSize();
        }
    }
    
    if( rtype == kRotateLandscape ) rate = 320/f_size.height; // landscape view;
    else                            rate = 320/f_size.width;  // portrait view;
    
    
    
    if( f_size.width > f_size.height ) wideValue = f_size.width;
    else wideValue = f_size.height;
    
    d_size.width = f_size.width * rate;
    
    d_size.height = f_size.height * rate;
    
    
    //    enum ResolutionPolicy rPolicy = kResolutionFixedHeight;
    //kResolutionExactFit;
    //    enum ResolutionPolicy rPolicy = kResolutionShowAll;
    //kResolutionNoBorder;
    
    enum ResolutionPolicy rPolicy = ResolutionPolicy::EXACT_FIT;
    
    switch ( platform ) {
        case Application::Platform::OS_IPHONE:
        {
            if( wideValue >= (960 - 100) ) factor = 2;
            
            if( d_size.width == 320 && d_size.height > 569 )
            {
                d_size.height = 568;
            }
            
            CCLOG("design size:: for iPhone :: %f x %f" , d_size.width , d_size.height );
            
        }break;
        case Application::Platform::OS_IPAD:
        {
            factor = 2;
            
            d_size.width = 320;
            d_size.height = 500;
            rPolicy = ResolutionPolicy::SHOW_ALL;
            
            CCLOG("design size:: for iPAD :: %f x %f" , d_size.width , d_size.height );
        }break;
        case Application::Platform::OS_ANDROID:
        {
            if ( Configuration::getInstance()->getMaxTextureSize() < 2048 )
            { // depending on texture size.
                factor = 1;
            }
            else
            {
                factor = 2;
            }
        }break;
        case Application::Platform::OS_MAC:
        {
            factor = 2;
            CCLOG("design size:: for Mac OS :: %f x %f" , d_size.width , d_size.height );
        }break;
            
        default:
        {
            
        }
            break;
    }
    
    //*/
    switch (factor)
    {
        case 1: FileUtils::getInstance()->addSearchPath("res-sd");
        {
            
        }
            break;
        case 2: FileUtils::getInstance()->addSearchPath("res");
        {
            
        }
            break;
    }
    
    glView->setDesignResolutionSize( d_size.width , d_size.height , rPolicy );
    
  //  glView->setDesignResolutionSize( 320 , 568 , ResolutionPolicy::SHOW_ALL );
    
    pDirector->setContentScaleFactor( factor );
    
}
















AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
    
    
    settingResoulution(kRotatePortrait);
    
    

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    register_all_packages();

    // create a scene. it's an autorelease object
    auto scene = MainScene::create();//MainScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
