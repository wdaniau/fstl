#include <QMenuBar>

#include "window.h"
#include "canvas.h"
#include "loader.h"
#include "shaderlightprefs.h"
#include "speedmousedialog.h"
#include <QDrag>

const QString Window::RECENT_FILE_KEY = "recentFiles";
const QString Window::INVERT_ZOOM_KEY = "invertZoom";
const QString Window::AUTORELOAD_KEY = "autoreload";
const QString Window::DRAW_AXES_KEY = "drawAxes";
const QString Window::PROJECTION_KEY = "projection";
const QString Window::DRAW_MODE_KEY = "drawMode";
const QString Window::WINDOW_GEOM_KEY = "windowGeometry";
const QString Window::RESET_TRANSFORM_ON_LOAD_KEY = "resetTransformOnLoad";
const QString Window::HIDE_MENU_BAR = "hideMenuBar";


const QKeySequence Window::shortcutOpen = Qt::Key_O;
const QKeySequence Window::shortcutReload = Qt::Key_R;
const QKeySequence Window::shortcutScreenshot = Qt::Key_S;
const QKeySequence Window::shortcutQuit = Qt::Key_Q;
const QKeySequence Window::shortcutDrawModeSettings = Qt::Key_P;
const QKeySequence Window::shortcutDrawAxes = Qt::Key_A;
const QKeySequence Window::shortcutHideMenuBar = Qt::Key_M;
const QKeySequence Window::shortcutFullscreen = Qt::Key_F;
const QKeySequence Window::shortcutHelp = Qt::Key_H;

const QKeySequence Window::shortcutCenterView = Qt::Key_C;
const QKeySequence Window::shortcutDefaultView = Qt::Key_0;
const QKeySequence Window::shortcutTopView = Qt::Key_1;
const QKeySequence Window::shortcutBottomView = Qt::Key_2;
const QKeySequence Window::shortcutFrontView = Qt::Key_3;
const QKeySequence Window::shortcutRearView = Qt::Key_4;
const QKeySequence Window::shortcutLeftView = Qt::Key_5;
const QKeySequence Window::shortcutRightView = Qt::Key_6;

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    open_action(new QAction("Open", this)),
    about_action(new QAction("About", this)),
    help_action(new QAction("Help",this)),
    quit_action(new QAction("Quit", this)),
    perspective_action(new QAction("Perspective", this)),
    orthographic_action(new QAction("Orthographic", this)),
    shaded_action(new QAction("Shaded", this)),
    wireframe_action(new QAction("Wireframe", this)),
    surfaceangle_action(new QAction("Surface Angle", this)),
    meshlight_action(new QAction("Shaded ambient and directive light source", this)),
    drawModePrefs_action(new QAction("Draw Mode Settings")),
    axes_action(new QAction("Draw Axes", this)),
    invert_zoom_action(new QAction("Invert Zoom", this)),
    reload_action(new QAction("Reload", this)),
    autoreload_action(new QAction("Autoreload", this)),
    save_screenshot_action(new QAction("Save Screenshot", this)),
    hide_menuBar_action(new QAction("Hide Menu Bar", this)),
    fullscreen_action(new QAction("Toggle Fullscreen",this)),
    resetTransformOnLoadAction(new QAction("Reset rotation on load",this)),
    setGLSizeAction(new QAction("Set Viewport Size",this)),
    recent_files(new QMenu("Open recent", this)),
    recent_files_group(new QActionGroup(this)),
    recent_files_clear_action(new QAction("Clear recent files", this)),
    watcher(new QFileSystemWatcher(this))

{
    // Define status tip for actions
    open_action->setStatusTip(open_action->toolTip());
    about_action->setStatusTip(about_action->toolTip());
    help_action->setStatusTip(help_action->toolTip());
    quit_action->setStatusTip(quit_action->toolTip());
    perspective_action->setStatusTip(perspective_action->toolTip());
    orthographic_action->setStatusTip(orthographic_action->toolTip());
    shaded_action->setStatusTip(shaded_action->toolTip());
    wireframe_action->setStatusTip(wireframe_action->toolTip());
    surfaceangle_action->setStatusTip(surfaceangle_action->toolTip());
    meshlight_action->setStatusTip(meshlight_action->toolTip());
    drawModePrefs_action->setStatusTip(drawModePrefs_action->toolTip());
    axes_action->setStatusTip(axes_action->toolTip());
    invert_zoom_action->setStatusTip(invert_zoom_action->toolTip());
    reload_action->setStatusTip("Reload the file");
    autoreload_action->setStatusTip("Automatically reload file on file change");
    save_screenshot_action->setStatusTip(save_screenshot_action->toolTip());
    hide_menuBar_action->setStatusTip(hide_menuBar_action->toolTip());
    fullscreen_action->setStatusTip(fullscreen_action->toolTip());
    resetTransformOnLoadAction->setStatusTip(resetTransformOnLoadAction->toolTip());
    setGLSizeAction->setStatusTip(setGLSizeAction->toolTip());
    recent_files_clear_action->setStatusTip(recent_files_clear_action->toolTip());

    setWindowTitle("fstl-e " FSTLE_VERSION);
    setWindowIcon(QIcon(":/qt/icons/fstl-e_64x64.png"));
    setAcceptDrops(true);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(2, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);

    QSurfaceFormat::setDefaultFormat(format);
    canvas = new Canvas(format, this);
    setCentralWidget(canvas);
    canvas->update();

    statusBar = new QStatusBar;

    meshlightprefs = new ShaderLightPrefs(this, canvas);
    speedMouseDialog = new SpeedMouseDialog(this, canvas, statusBar);

    QObject::connect(drawModePrefs_action, &QAction::triggered,this,&Window::on_drawModePrefs);

    QObject::connect(watcher, &QFileSystemWatcher::fileChanged,
                     this, &Window::on_watched_change);

    //open_action->setShortcut(QKeySequence::Open);
    open_action->setShortcut(shortcutOpen);
    open_action->setIcon(QIcon(":/qt/icons/document-open.png"));
    QObject::connect(open_action, &QAction::triggered,
                     this, &Window::on_open);
    this->addAction(open_action);

    //quit_action->setShortcut(QKeySequence::Quit);
    quit_action->setShortcut(shortcutQuit);
    quit_action->setIcon(QIcon(":/qt/icons/exit.png"));
    QObject::connect(quit_action, &QAction::triggered,
                     this, &Window::close);
    this->addAction(quit_action);

    autoreload_action->setCheckable(true);
    autoreload_action->setIcon(QIcon(":/qt/icons/auto_refresh.png"));
    QObject::connect(autoreload_action, &QAction::triggered,
            this, &Window::on_autoreload_triggered);

    //reload_action->setShortcut(QKeySequence::Refresh);
    reload_action->setShortcut(shortcutReload);
    reload_action->setIcon(QIcon(":/qt/icons/view-refresh.png"));
    this->addAction(reload_action);
    reload_action->setEnabled(false);
    QObject::connect(reload_action, &QAction::triggered,
                     this, &Window::on_reload);


    about_action->setIcon(QIcon(":/qt/icons/fstl-e_64x64.png"));
    QObject::connect(about_action, &QAction::triggered,
                     this, &Window::on_about);

    help_action->setIcon(QIcon(":/qt/icons/help-browser.png"));
    help_action->setShortcut(shortcutHelp);
    this->addAction(help_action);
    connect(help_action,SIGNAL(triggered()),this,SLOT(on_help()));

    QObject::connect(recent_files_clear_action, &QAction::triggered,
                     this, &Window::on_clear_recent);
    QObject::connect(recent_files_group, &QActionGroup::triggered,
                     this, &Window::on_load_recent);

    save_screenshot_action->setCheckable(false);
    save_screenshot_action->setShortcut(shortcutScreenshot);
    save_screenshot_action->setIcon(QIcon(":/qt/icons/screenshot.png"));
    this->addAction(save_screenshot_action);
    QObject::connect(save_screenshot_action, &QAction::triggered, 
        this, &Window::on_save_screenshot);
    
    rebuild_recent_files();

    auto file_menu = menuBar()->addMenu("File");
    file_menu->addAction(open_action);
    file_menu->addMenu(recent_files);
    file_menu->addSeparator();
    file_menu->addAction(reload_action);
    file_menu->addAction(autoreload_action);
    file_menu->addAction(save_screenshot_action);
    file_menu->addAction(quit_action);

    auto view_menu = menuBar()->addMenu("View");
    projection_menu = view_menu->addMenu("Projection");
    projection_menu->addAction(perspective_action);
    perspective_action->setIcon(QIcon(":/qt/icons/perspective.png"));
    projection_menu->addAction(orthographic_action);
    orthographic_action->setIcon(QIcon(":/qt/icons/orthographic.png"));
    auto projections = new QActionGroup(projection_menu);
    for (auto p : {perspective_action, orthographic_action})
    {
        projections->addAction(p);
        p->setCheckable(true);
    }
    projections->setExclusive(true);
    QObject::connect(projections, &QActionGroup::triggered,
                     this, &Window::on_projection);

    defaultViewMenu = view_menu->addMenu("Default load view");
    QAction* default1ViewAction = new QAction(QIcon(":/qt/icons/default_1_64x64.png"),"Default 1");
    default1ViewAction->setStatusTip("Define default load view model 1");
    QAction* default2ViewAction = new QAction(QIcon(":/qt/icons/default_2_64x64.png"),"Default 2");
    default2ViewAction->setStatusTip("Define default load view model 2");
    QAction* default3ViewAction = new QAction(QIcon(":/qt/icons/default_3_64x64.png"),"Default 3");
    default3ViewAction->setStatusTip("Define default load view model 3");
    defaultViewMenu->addAction(default1ViewAction);
    defaultViewMenu->addAction(default2ViewAction);
    defaultViewMenu->addAction(default3ViewAction);
    defaultViewAction = new QActionGroup(defaultViewMenu);
    for (QAction* p : {default1ViewAction, default2ViewAction, default3ViewAction}) {
        defaultViewAction->addAction(p);
        p->setCheckable(true);
    }
    defaultViewAction->setExclusive(true);
    QObject::connect(defaultViewAction, &QActionGroup::triggered,
                     this, &Window::on_defaultView);

    draw_menu = view_menu->addMenu("Draw Mode");
    draw_menu->addAction(shaded_action);
    draw_menu->addAction(wireframe_action);
    draw_menu->addAction(surfaceangle_action);
    draw_menu->addAction(meshlight_action);
    shaded_action->setIcon(QIcon(":/qt/icons/sphere_shader1.png"));
    wireframe_action->setIcon(QIcon(":/qt/icons/sphere_shader2.png"));
    surfaceangle_action->setIcon(QIcon(":/qt/icons/sphere_shader3.png"));
    meshlight_action->setIcon(QIcon(":/qt/icons/sphere_shader4.png"));
    auto drawModes = new QActionGroup(draw_menu);
    for (auto p : {shaded_action, wireframe_action, surfaceangle_action, meshlight_action})
    {
        drawModes->addAction(p);
        p->setCheckable(true);
    }
    drawModes->setExclusive(true);
    QObject::connect(drawModes, &QActionGroup::triggered,
                     this, &Window::on_drawMode);
    view_menu->addAction(drawModePrefs_action);
    drawModePrefs_action->setShortcut(shortcutDrawModeSettings);
    drawModePrefs_action->setIcon(QIcon(":/qt/icons/preferences-system.png"));
    this->addAction(drawModePrefs_action);
    drawModePrefs_action->setDisabled(true);
    view_menu->addAction(axes_action);
    axes_action->setCheckable(true);
    axes_action->setShortcut(shortcutDrawAxes);
    axes_action->setIcon(QIcon(":/qt/icons/axes.png"));
    this->addAction(axes_action);
    QObject::connect(axes_action, &QAction::toggled,
            this, &Window::on_drawAxes);

    view_menu->addAction(invert_zoom_action);
    invert_zoom_action->setCheckable(true);
    invert_zoom_action->setIcon(QIcon(":/qt/icons/invert_zoom.png"));
    QObject::connect(invert_zoom_action, &QAction::triggered,
            this, &Window::on_invertZoom);       

    view_menu->addAction(resetTransformOnLoadAction);
    resetTransformOnLoadAction->setCheckable(true);
    resetTransformOnLoadAction->setIcon(QIcon(":/qt/icons/reset_rotation_on_load.png"));
    QObject::connect(resetTransformOnLoadAction, &QAction::triggered,
            this, &Window::on_resetTransformOnLoad);

    view_menu->addAction(hide_menuBar_action);
    hide_menuBar_action->setShortcut(shortcutHideMenuBar);
    hide_menuBar_action->setCheckable(true);
    QObject::connect(hide_menuBar_action, &QAction::toggled,
            this, &Window::on_hide_menuBar);
    // To have the shortcut work without the menu
    this->addAction(hide_menuBar_action);

    view_menu->addAction(fullscreen_action);
    fullscreen_action->setShortcut(shortcutFullscreen);
    fullscreen_action->setIcon(QIcon(":/qt/icons/view-fullscreen.png"));
    fullscreen_action->setCheckable(true);
    QObject::connect(fullscreen_action, &QAction::toggled,
            this, &Window::on_fullscreen);
    this->addAction(fullscreen_action);

    QMenu *resolutionMenu = view_menu->addMenu("Set Viewport Size");
    resolutionMenu->setIcon(QIcon(":/qt/icons/resolution_1_32.png"));
    resolutionMenu->menuAction()->setIconVisibleInMenu(true);
    QActionGroup* groupResolution = new QActionGroup(resolutionMenu);

    QAction *quatreTiers = new QAction("-- 4:3",this);
    quatreTiers->setDisabled(true);
    resolutionMenu->addAction(quatreTiers);
    QAction *setResolution0Action = new QAction("640 x 480  (VGA)",this);
    resolutionMenu->addAction(setResolution0Action);
    groupResolution->addAction(setResolution0Action);

    QAction *setResolution1Action = new QAction("768 x 576  (PAL)",this);
    resolutionMenu->addAction(setResolution1Action);
    groupResolution->addAction(setResolution1Action);

    QAction *setResolution2Action = new QAction("800 x 600  (SVGA)",this);
    resolutionMenu->addAction(setResolution2Action);
    groupResolution->addAction(setResolution2Action);

    QAction *setResolution3Action = new QAction("1024 x 768 (XGA)",this);
    resolutionMenu->addAction(setResolution3Action);
    groupResolution->addAction(setResolution3Action);

    QAction *seizeNeuf = new QAction("-- 16:9",this);
    seizeNeuf->setDisabled(true);
    resolutionMenu->addAction(seizeNeuf);

    QAction *setResolution4Action = new QAction("800 x 480  (WVGA)",this);
    resolutionMenu->addAction(setResolution4Action);
    groupResolution->addAction(setResolution4Action);

    QAction *setResolution5Action = new QAction("1024 x 576 (16:9 PAL)",this);
    resolutionMenu->addAction(setResolution5Action);
    groupResolution->addAction(setResolution5Action);

    QAction *setResolution6Action = new QAction("1280 x 720 (HD720)",this);
    resolutionMenu->addAction(setResolution6Action);
    groupResolution->addAction(setResolution6Action);

    QAction *setResolution7Action = new QAction("1920 x 1080 (HD1080)",this);
    resolutionMenu->addAction(setResolution7Action);
    groupResolution->addAction(setResolution7Action);

    resolutionMenu->addSeparator();

//    QAction *setCustomResolutionAction = new QAction("Custom Resolution",this);
//    //connect(setCustomResolutionAction,SIGNAL(triggered(bool)),this,SLOT(setCustomResolution()));
//    resolutionMenu->addAction(setCustomResolutionAction);

    connect(groupResolution,SIGNAL(triggered(QAction*)),this,SLOT(setViewportSize(QAction*)));

    centerAction = new QAction("Center View");
    centerAction->setStatusTip(centerAction->toolTip());
    centerAction->setShortcut(shortcutCenterView);
    centerAction->setIcon(QIcon(":/qt/icons/center_64x64.png"));
    centerAction->setCheckable(false);
    this->addAction(centerAction);
    view_menu->addAction(centerAction);
    QObject::connect(centerAction, &QAction::triggered,
                     this, &Window::on_centerView);


    QMenu *applyViewMenu = view_menu->addMenu("Apply Specific View");
    applyViewMenu->setIcon(QIcon(":/qt/icons/eye_64x64.png"));
    applyViewMenu->menuAction()->setIconVisibleInMenu(true);
    QActionGroup* groupApplyViewAction = new QActionGroup(applyViewMenu);

    applyDefaultViewAction = new QAction("Default View");
    applyDefaultViewAction->setStatusTip(applyDefaultViewAction->toolTip());
    applyDefaultViewAction->setIcon(defaultViewMenu->icon());
    applyDefaultViewAction->setShortcut(shortcutDefaultView);
    this->addAction(applyDefaultViewAction);
    applyViewMenu->addAction(applyDefaultViewAction);
    groupApplyViewAction->addAction(applyDefaultViewAction);

    QAction* applyTopViewAction = new QAction("Top View");
    applyTopViewAction->setStatusTip(applyTopViewAction->toolTip());
    applyTopViewAction->setIcon(QIcon(":/qt/icons/top_XY_64x64.png"));
    applyTopViewAction->setShortcut(shortcutTopView);
    this->addAction(applyTopViewAction);
    applyViewMenu->addAction(applyTopViewAction);
    groupApplyViewAction->addAction(applyTopViewAction);

    QAction* applyBottomViewAction = new QAction("Bottom View");
    applyBottomViewAction->setStatusTip(applyBottomViewAction->toolTip());
    applyBottomViewAction->setIcon(QIcon(":/qt/icons/bottom_X-Y_64x64.png"));
    applyBottomViewAction->setShortcut(shortcutBottomView);
    this->addAction(applyBottomViewAction);
    applyViewMenu->addAction(applyBottomViewAction);
    groupApplyViewAction->addAction(applyBottomViewAction);

    QAction* applyFrontViewAction = new QAction("Front View");
    applyFrontViewAction->setStatusTip(applyFrontViewAction->toolTip());
    applyFrontViewAction->setIcon(QIcon(":/qt/icons/front_XZ_64x64.png"));
    applyFrontViewAction->setShortcut(shortcutFrontView);
    this->addAction(applyFrontViewAction);
    applyViewMenu->addAction(applyFrontViewAction);
    groupApplyViewAction->addAction(applyFrontViewAction);

    QAction* applyRearViewAction = new QAction("Rear View");
    applyRearViewAction->setStatusTip(applyRearViewAction->toolTip());
    applyRearViewAction->setIcon(QIcon(":/qt/icons/rear_-XZ_64x64.png"));
    applyRearViewAction->setShortcut(shortcutRearView);
    this->addAction(applyRearViewAction);
    applyViewMenu->addAction(applyRearViewAction);
    groupApplyViewAction->addAction(applyRearViewAction);

    QAction* applyLeftViewAction = new QAction("Left View");
    applyLeftViewAction->setStatusTip(applyLeftViewAction->toolTip());
    applyLeftViewAction->setIcon(QIcon(":/qt/icons/left_-YZ_64x64.png"));
    applyLeftViewAction->setShortcut(shortcutLeftView);
    this->addAction(applyLeftViewAction);
    applyViewMenu->addAction(applyLeftViewAction);
    groupApplyViewAction->addAction(applyLeftViewAction);

    QAction* applyRightViewAction = new QAction("Right View");
    applyRightViewAction->setStatusTip(applyRightViewAction->toolTip());
    applyRightViewAction->setIcon(QIcon(":/qt/icons/right_YZ_64x64.png"));
    applyRightViewAction->setShortcut(shortcutRightView);
    this->addAction(applyRightViewAction);
    applyViewMenu->addAction(applyRightViewAction);
    groupApplyViewAction->addAction(applyRightViewAction);

    connect(groupApplyViewAction,SIGNAL(triggered(QAction*)),this,SLOT(onApplyView(QAction*)));


    QMenu *msaaMenu = view_menu->addMenu("Anti-aliasing");
    //aaMenu->setIcon(QIcon(":/qt/icons/eye_64x64.png"));
    //aaMenu->menuAction()->setIconVisibleInMenu(true);
    QActionGroup* groupMsaaAction = new QActionGroup(msaaMenu);
    QAction* noMsaaAction = new QAction("No AA");
    noMsaaAction->setData(-1);
    QAction* msaa2Action = new QAction("2x");
    msaa2Action->setData(2);
    QAction* msaa4Action = new QAction("4x");
    msaa4Action->setData(4);
    QAction* msaa8Action = new QAction("8x");
    msaa8Action->setData(8);
    groupMsaaAction->setExclusive(true);
    for (QAction* p : {noMsaaAction, msaa2Action, msaa4Action, msaa8Action})
    {
        msaaMenu->addAction(p);
        groupMsaaAction->addAction(p);
        p->setCheckable(true);
    }
    // Find msaa value and set action
    int currentMsaa = canvas->getMsaa();
    for (QAction* p : groupMsaaAction->actions()) {
        if (p->data().toInt()==currentMsaa) {
            p->setChecked(true);
            break;
        }
    }
    // do the work
    connect(groupMsaaAction,SIGNAL(triggered(QAction*)),this,SLOT(onMsaaAction(QAction*)));

    auto help_menu = menuBar()->addMenu("Help");
    help_menu->addAction(about_action);
    help_menu->addAction(help_action);

    // Toolbar
    // First group
    windowToolBar = new QToolBar;
    windowToolBar->addAction(quit_action);
    windowToolBar->addAction(open_action);
    windowToolBar->addAction(reload_action);
    windowToolBar->addAction(autoreload_action);

    // preferences button here
    windowToolBar->addSeparator();

    // Second group
    projectionButton = new QToolButton;
    projectionButton->setPopupMode(QToolButton::InstantPopup);
    projectionButton->setMenu(projection_menu);
    projectionButton->setFocusPolicy(Qt::NoFocus); // we do not want the button to have keyboard focus
    windowToolBar->addWidget(projectionButton);

    defaultViewButton = new QToolButton;
    defaultViewButton->setPopupMode(QToolButton::InstantPopup);
    defaultViewButton->setMenu(defaultViewMenu);
    defaultViewButton->setFocusPolicy(Qt::NoFocus);
    windowToolBar->addWidget(defaultViewButton);

    shaderButton = new QToolButton;
    shaderButton->setPopupMode(QToolButton::InstantPopup);
    shaderButton->setMenu(draw_menu);
    shaderButton->setFocusPolicy(Qt::NoFocus); // we do not want the button to have keyboard focus
    windowToolBar->addWidget(shaderButton);
    windowToolBar->addAction(drawModePrefs_action);

    windowToolBar->addAction(axes_action);
    windowToolBar->addAction(invert_zoom_action);
    windowToolBar->addAction(resetTransformOnLoadAction);

    windowToolBar->addSeparator();
    // Third group

    QToolButton* viewportSizeButton = new QToolButton;
    viewportSizeButton->setPopupMode(QToolButton::InstantPopup);
    viewportSizeButton->setMenu(resolutionMenu);
    viewportSizeButton->setIcon(resolutionMenu->icon());
    viewportSizeButton->setToolTip(resolutionMenu->title());
    viewportSizeButton->setFocusPolicy(Qt::NoFocus); // we do not want the button to have keyboard focus
    viewportSizeButton->setStatusTip(viewportSizeButton->toolTip());
    windowToolBar->addWidget(viewportSizeButton);

    windowToolBar->addAction(centerAction);

    QToolButton* applyViewButton = new QToolButton;
    applyViewButton->setPopupMode(QToolButton::InstantPopup);
    applyViewButton->setMenu(applyViewMenu);
    applyViewButton->setIcon(applyViewMenu->icon());
    applyViewButton->setToolTip(applyViewMenu->title());
    applyViewButton->setFocusPolicy(Qt::NoFocus); // we do not want the button to have keyboard focus
    applyViewButton->setStatusTip(applyViewButton->toolTip());
    windowToolBar->addWidget(applyViewButton);



    windowToolBar->addAction(save_screenshot_action);
    windowToolBar->addAction(fullscreen_action);

    windowToolBar->addSeparator();

    //
    speedMouseButton = new QToolButton;
    speedMouseButton->setIcon(QIcon(":/qt/icons/speed_mouse_64x64.png"));
    speedMouseButton->setToolTip("Toggle mouse speed adjustment");
    speedMouseButton->setStatusTip(speedMouseButton->toolTip());
    speedMouseButton->setFocusPolicy(Qt::NoFocus);
    windowToolBar->addWidget(speedMouseButton);
    connect(speedMouseButton,SIGNAL(clicked(bool)),this,SLOT(onSpeedMouseButton()));

    windowToolBar->addSeparator();
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    windowToolBar->addWidget(spacer);
    windowToolBar->addAction(help_action);


    this->addToolBar(windowToolBar);


    filenameStatusLabel = new QLabel("File:none");
    filenameStatusLabel->setStatusTip("Current file (click and hold to drop to another application)");
    statusBar->addPermanentWidget(filenameStatusLabel);
    this->setStatusBar(statusBar);

    load_persist_settings();
}

void Window::load_persist_settings(){
    QSettings settings;
    bool invert_zoom = settings.value(INVERT_ZOOM_KEY, false).toBool();
    canvas->invert_zoom(invert_zoom);
    invert_zoom_action->setChecked(invert_zoom);

    bool resetTransformOnLoad = settings.value(RESET_TRANSFORM_ON_LOAD_KEY, true).toBool();
    canvas->setResetTransformOnLoad(resetTransformOnLoad);
    resetTransformOnLoadAction->setChecked(resetTransformOnLoad);

    autoreload_action->setChecked(settings.value(AUTORELOAD_KEY, true).toBool());

    bool draw_axes = settings.value(DRAW_AXES_KEY, false).toBool();
    canvas->draw_axes(draw_axes);
    axes_action->setChecked(draw_axes);

    QString projection = settings.value(PROJECTION_KEY, "perspective").toString();
    QAction* currentProjection;
    if(projection == "perspective"){
        canvas->view_perspective(Canvas::P_PERSPECTIVE, false);
        currentProjection = perspective_action;
    }else{
        canvas->view_perspective(Canvas::P_ORTHOGRAPHIC, false);
        currentProjection = orthographic_action;
    }
    currentProjection->setChecked(true);
    on_projection(currentProjection);

    QString defaultView = canvas->getDefaultView();
    for (QAction* a: defaultViewAction->actions()) {
        if (a->text().toLower() == defaultView.toLower()) {
            a->setChecked(true);
            on_defaultView(a);
            break;
        }
    }

    DrawMode draw_mode = (DrawMode)settings.value(DRAW_MODE_KEY, meshlight).toInt();
    
    if(draw_mode >= DRAWMODECOUNT)
    {
        draw_mode = shaded;
    }
    dm_acts = {shaded_action, wireframe_action, surfaceangle_action, meshlight_action};
    dm_acts[draw_mode]->setChecked(true);
    on_drawMode(dm_acts[draw_mode]);

    // menu bar
    bool hideMenu = settings.value(HIDE_MENU_BAR, false).toBool();
    hide_menuBar_action->blockSignals(true);
    hide_menuBar_action->setChecked(hideMenu);
    on_hide_menuBar();
    hide_menuBar_action->blockSignals(false);

    resize(600, 400);
    restoreGeometry(settings.value(WINDOW_GEOM_KEY).toByteArray());
    if (this->isFullScreen()) {
        fullscreen_action->blockSignals(true);
        fullscreen_action->setChecked(true);
        fullscreen_action->blockSignals(false);
    }
 }

void Window::on_drawModePrefs() {
    // For now only one draw mode has settings
    // when settings for other draw mode will be available
    // we will need to check the current mode
    if (meshlightprefs->isVisible()) {
        meshlightprefs->hide();
    } else {
        meshlightprefs->show();
    }
}

void Window::on_open()
{
    QSettings settings;
    QString lastDir = QString();
    QStringList recentFileList = settings.value(RECENT_FILE_KEY).toStringList();
    if (!recentFileList.isEmpty()) {
        lastDir = QFileInfo(recentFileList.at(0)).absoluteDir().absolutePath();
    }

    const QString filename = QFileDialog::getOpenFileName(
                this, "Load .stl file", lastDir, "STL files (*.stl *.STL)");
    if (!filename.isNull())
    {
        load_stl(filename);
    }
}

void Window::on_about()
{
    QMessageBox::about(this, "",
                       "<p align=\"center\">This is <b>fstl-e</b><br>" FSTLE_VERSION "</p>"
                       "<p>A fast viewer for <code>.stl</code> files.</p>"
                       "<p>source code of this version available here :"
                       "<a href=\"https://github.com/wdaniau/fstl\""
                       "   style=\"color: #93a1a1;\">https://github.com/wdaniau/fstl</a></p>"
                       "<font size='small'>"
                       "<p>It is a forked version of <b>fstl</b> 0.10.0<br>"
                       "with some fancy enhancements"
                       "</p>"
                       "<p>Original version © 2014-2024 Matthew Keeter<br>"
                       "<a href=\"https://github.com/fstl-app/fstl\""
                       "   style=\"color: #93a1a1;\">https://github.com/fstl-app/fstl</a><br>"
                       "<a href=\"mailto:matt.j.keeter@gmail.com\""
                       "   style=\"color: #93a1a1;\">matt.j.keeter@gmail.com</a></p>"
                       "</font>"
                       );
}

void Window::on_bad_stl()
{
    QMessageBox::critical(this, "Error",
                          "<b>Error:</b><br>"
                          "This <code>.stl</code> file is invalid or corrupted.<br>"
                          "Please export it from the original source, verify, and retry.");
    load_stl(":/gl/sphere.stl");
    filenameStatusLabel->setText("File:none");
}

void Window::on_empty_mesh()
{
    QMessageBox::critical(this, "Error",
                          "<b>Error:</b><br>"
                          "This file is syntactically correct<br>but contains no triangles.");
    load_stl(":/gl/sphere.stl");
    filenameStatusLabel->setText("File:none");
}

void Window::on_missing_file()
{
    QMessageBox::critical(this, "Error",
                          "<b>Error:</b><br>"
                          "The target file is missing.<br>");
    load_stl(":/gl/sphere.stl");
    filenameStatusLabel->setText("File:none");
}

void Window::enable_open()
{
    open_action->setEnabled(true);
}

void Window::disable_open()
{
    open_action->setEnabled(false);
}

void Window::set_watched(const QString& filename)
{
    const auto files = watcher->files();
    if (files.size())
    {
        watcher->removePaths(watcher->files());
    }
    watcher->addPath(filename);

    QSettings settings;
    auto recent = settings.value(RECENT_FILE_KEY).toStringList();
    const auto f = QFileInfo(filename).absoluteFilePath();
    recent.removeAll(f);
    recent.prepend(f);
    while (recent.size() > MAX_RECENT_FILES)
    {
        recent.pop_back();
    }
    settings.setValue(RECENT_FILE_KEY, recent);
    rebuild_recent_files();
}

void Window::on_projection(QAction* proj)
{
    if (proj == perspective_action)
    {
        canvas->view_perspective(Canvas::P_PERSPECTIVE, true);
        QSettings().setValue(PROJECTION_KEY, "perspective");
    }
    else
    {
        canvas->view_perspective(Canvas::P_ORTHOGRAPHIC, true);
        QSettings().setValue(PROJECTION_KEY, "orthographic");
    }
    projection_menu->setIcon(proj->icon());
    projectionButton->setIcon(proj->icon());
    projectionButton->setToolTip(QString("%1 : %2").arg(projection_menu->title()).arg(proj->toolTip()));
    projectionButton->setStatusTip(projectionButton->toolTip());
}

void Window::on_drawMode(QAction* act)
{
    // On mode change hide prefs first
    meshlightprefs->hide();

    DrawMode mode;
    if (act == shaded_action)
    {
        drawModePrefs_action->setEnabled(false);
        mode = shaded;
    }
    else if (act == wireframe_action)
    {
        drawModePrefs_action->setEnabled(false);
        mode = wireframe;
    }
    else if (act == surfaceangle_action)
    {
        drawModePrefs_action->setEnabled(false);
        mode = surfaceangle;
    }
    else if (act == meshlight_action)
    {
        drawModePrefs_action->setEnabled(true);
        mode = meshlight;
    }
    canvas->set_drawMode(mode);
    QSettings().setValue(DRAW_MODE_KEY, mode);
    draw_menu->setIcon(act->icon());
    shaderButton->setIcon(act->icon());
    shaderButton->setToolTip(QString("%1 : %2").arg(draw_menu->title()).arg(act->toolTip()));
    shaderButton->setStatusTip(shaderButton->toolTip());
}

void Window::on_defaultView(QAction* view) {
    canvas->setDefaultView(view->text());
    defaultViewMenu->setIcon(view->icon());
    defaultViewButton->setIcon(view->icon());
    defaultViewButton->setToolTip(QString("Default load view : %1").arg(view->toolTip()));
    defaultViewButton->setStatusTip(QString("Default load view : %1").arg(view->toolTip()));
    canvas->setDefaultView(view->text());
    applyDefaultViewAction->setIcon(defaultViewButton->icon());
}

void Window::on_drawAxes(bool d)
{
    canvas->draw_axes(d);
    QSettings().setValue(DRAW_AXES_KEY, d);
}

void Window::on_invertZoom(bool d)
{
    canvas->invert_zoom(d);
    QSettings().setValue(INVERT_ZOOM_KEY, d);
}

void Window::on_resetTransformOnLoad(bool d) {
    canvas->setResetTransformOnLoad(d);
    QSettings().setValue(RESET_TRANSFORM_ON_LOAD_KEY, d);
}

void Window::on_watched_change(const QString& filename)
{
    if (autoreload_action->isChecked())
    {
        load_stl(filename, true);
    }
}

void Window::on_autoreload_triggered(bool b)
{
    if (b)
    {
        on_reload();
    }
    QSettings().setValue(AUTORELOAD_KEY, b);
}

void Window::on_clear_recent()
{
    QSettings settings;
    settings.setValue(RECENT_FILE_KEY, QStringList());
    rebuild_recent_files();
}

void Window::on_load_recent(QAction* a)
{
    load_stl(a->data().toString());
}

void Window::on_loaded(const QString& filename)
{
    current_file = filename;
    QFileInfo fileInfo = QFileInfo(current_file);
    filenameStatusLabel->setText("File:"+fileInfo.fileName());
}

void Window::on_save_screenshot()
{
    const auto image = canvas->grabFramebuffer();
    auto file_name = QFileDialog::getSaveFileName(
        this, 
        tr("Save Screenshot Image"),
        QStandardPaths::standardLocations(QStandardPaths::StandardLocation::PicturesLocation).first(),
        "Images (*.png *.jpg)");

    auto get_file_extension = [](const std::string& file_name) -> std::string
    {
        const auto location = std::find(file_name.rbegin(), file_name.rend(), '.');
        if (location == file_name.rend())
        {
            return "";
        }

        const auto index = std::distance(file_name.rbegin(), location);
        return file_name.substr(file_name.size() - index);
    };

    const auto extension = get_file_extension(file_name.toStdString());
    if(extension.empty() || (extension != "png" && extension != "jpg"))
    {
        file_name.append(".png");
    }
    
    const auto save_ok = image.save(file_name);
    if(!save_ok)
    {
        QMessageBox::warning(this, tr("Error Saving Image"), tr("Unable to save screen shot image."));
    }
}

void Window::on_hide_menuBar()
{
    menuBar()->setVisible(!hide_menuBar_action->isChecked());
    windowToolBar->setVisible(!hide_menuBar_action->isChecked());
    statusBar->setVisible(!hide_menuBar_action->isChecked());
    QSettings settings;
    settings.setValue(HIDE_MENU_BAR,hide_menuBar_action->isChecked());
}

void Window::rebuild_recent_files()
{
    QSettings settings;
    QStringList files = settings.value(RECENT_FILE_KEY).toStringList();

    const auto actions = recent_files_group->actions();
    for (auto a : actions)
    {
        recent_files_group->removeAction(a);
    }
    recent_files->clear();

    for (auto f : files)
    {
        const auto a = new QAction(f, recent_files);
        a->setData(f);
        recent_files_group->addAction(a);
        recent_files->addAction(a);
    }
    if (files.size() == 0)
    {
        auto a = new QAction("No recent files", recent_files);
        recent_files->addAction(a);
        a->setEnabled(false);
    }
    recent_files->addSeparator();
    recent_files->addAction(recent_files_clear_action);
}

void Window::on_reload()
{
    auto fs = watcher->files();
    if (fs.size() == 1)
    {
        load_stl(fs[0], true);
    }
}


bool Window::load_stl(QString filename, bool is_reload)
{
    if (!open_action->isEnabled())  return false;

    // is it a directory?
    bool isDir = QFileInfo(filename).isDir();
    if (isDir) {
        QDir searchDir = QDir(filename, "*.stl",QDir::SortFlag::DirsFirst | QDir::SortFlag::Name);
        QStringList listFiles = searchDir.entryList();
        if (!listFiles.isEmpty()) {
            filename = filename+"/"+listFiles.at(0);
        }
    }

    canvas->set_status("Loading " + filename);

    Loader* loader = new Loader(this, filename, is_reload);
    connect(loader, &Loader::started,
              this, &Window::disable_open);

    connect(loader, &Loader::got_mesh,
            canvas, &Canvas::load_mesh);
    connect(loader, &Loader::error_bad_stl,
              this, &Window::on_bad_stl);
    connect(loader, &Loader::error_empty_mesh,
              this, &Window::on_empty_mesh);
    connect(loader, &Loader::error_missing_file,
              this, &Window::on_missing_file);

    connect(loader, &Loader::finished,
            loader, &Loader::deleteLater);
    connect(loader, &Loader::finished,
              this, &Window::enable_open);
    connect(loader, &Loader::finished,
            canvas, &Canvas::clear_status);

    if (filename[0] != ':')
    {
        //connect(loader, &Loader::loaded_file,
        //          this, &Window::setWindowTitle);

        connect(loader, &Loader::loaded_file,
                  this, &Window::set_watched);
        connect(loader, &Loader::loaded_file,
                  this, &Window::on_loaded);
        reload_action->setEnabled(true);
    }

    loader->start();
    return true;
}

void Window::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        auto urls = event->mimeData()->urls();
        if (urls.size() == 1 && urls.front().path().endsWith(".stl"))
            event->acceptProposedAction();
    }
}

void Window::dropEvent(QDropEvent *event)
{
    load_stl(event->mimeData()->urls().front().toLocalFile());
}

void Window::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && filenameStatusLabel->underMouse() && !current_file.isEmpty()) {
        // we do not want to drop on ourselves
        this->setAcceptDrops(false);
        QDrag* drag = new QDrag(this);
        QMimeData* mimeData = new QMimeData;
        QList<QUrl> urls = QList<QUrl>()<< QUrl::fromLocalFile(current_file);
        mimeData->setUrls(urls);
        drag->setMimeData(mimeData);
        drag->setPixmap(QPixmap(":/qt/icons/fstl-e_64x64.png").scaledToWidth(32));
        //drag->exec();
        // Force LinkAction in exec to avoid moving files to desktop under gnome :-(
        Qt::DropAction dropAction = drag->exec(Qt::LinkAction);
        //qDebug() << dropAction;

        // accept drops again
        this->setAcceptDrops(true);
    }
    QMainWindow::mousePressEvent(event);
}

void Window::resizeEvent(QResizeEvent *event)
{
    QSettings().setValue(WINDOW_GEOM_KEY, saveGeometry());
    if (speedMouseDialog->isVisible()) {
        speedMouseDialog->hide();
    }
    QWidget::resizeEvent(event);
}

void Window::moveEvent(QMoveEvent *event)
{
    QSettings().setValue(WINDOW_GEOM_KEY, saveGeometry());
    if (speedMouseDialog->isVisible()) {
        speedMouseDialog->hide();
    }
    QWidget::moveEvent(event);
}

void Window::sorted_insert(QStringList& list, const QCollator& collator, const QString& value)
{
    int start = 0;
    int end = list.size() - 1;
    int index = 0;
    while (start <= end){
        int mid = (start+end)/2;
        if (list[mid] == value) {
            return;
        }
        int compare = collator.compare(value, list[mid]);
        if (compare < 0) {
            end = mid-1;
            index = mid;
        } else {
            start = mid+1;
            index = start;
        }
    }

    list.insert(index, value);
}

void Window::build_folder_file_list()
{
    QString current_folder_path = QFileInfo(current_file).absoluteDir().absolutePath();
    if (!lookup_folder_files.isEmpty())
    {
        if (current_folder_path == lookup_folder) {
            return;
        }

        lookup_folder_files.clear();
    }
    lookup_folder = current_folder_path;

    QCollator collator;
    collator.setNumericMode(true);

    QDirIterator dirIterator(lookup_folder, QStringList() << "*.stl", QDir::Files | QDir::Readable | QDir::Hidden);
    while (dirIterator.hasNext()) {
        dirIterator.next();

        QString name = dirIterator.fileName();
        sorted_insert(lookup_folder_files, collator, name);
    }
}

QPair<QString, QString> Window::get_file_neighbors()
{
    if (current_file.isEmpty()) {
        return QPair<QString, QString>(QString(), QString());
    }

    build_folder_file_list();

    QFileInfo fileInfo(current_file);

    QString current_dir = fileInfo.absoluteDir().absolutePath();
    QString current_name = fileInfo.fileName();

    QString prev = QString();
    QString next = QString();

    QListIterator<QString> fileIterator(lookup_folder_files);
    while (fileIterator.hasNext()) {
        QString name = fileIterator.next();

        if (name == current_name) {
            if (fileIterator.hasNext()) {
                next = current_dir + QDir::separator() + fileIterator.next();
            }
            break;
        }

        prev = name;
    }

    if (!prev.isEmpty()) {
        prev.prepend(QDir::separator());
        prev.prepend(current_dir);
    }

    return QPair<QString, QString>(prev, next);
}

bool Window::load_prev(void)
{
    QPair<QString, QString> neighbors = get_file_neighbors();
    if (neighbors.first.isEmpty()) {
        return false;
    }

    return load_stl(neighbors.first);
}

bool Window::load_next(void)
{
    QPair<QString, QString> neighbors = get_file_neighbors();
    if (neighbors.second.isEmpty()) {
        return false;
    }

    return load_stl(neighbors.second);
}

void Window::keyPressEvent(QKeyEvent* event)
{
    if (!open_action->isEnabled())
    {
        QMainWindow::keyPressEvent(event);
        return;
    }

    if (event->key() == Qt::Key_Left)
    {
        load_prev();
        return;
    }
    else if (event->key() == Qt::Key_Right)
    {
        load_next();
        return;
    } else if (event->key() == Qt::Key_Up) {
        cycleShader(true);
        return;
    } else if (event->key() == Qt::Key_Down) {
        cycleShader(false);
        return;
    } else if (event->key() == Qt::Key_Escape && !menuBar()->isVisible()) { // this is if user did not noticed the hide menu key
        hide_menuBar_action->toggle();
        return;
    } else if (event->key() == Qt::Key_Escape && speedMouseDialog->isVisible()) {
        speedMouseDialog->hide();
        return;
    } else if (event->key() == Qt::Key_W) {
        if (dm_acts.at(getCurrentShader()) == meshlight_action) {
            meshlightprefs->toggleUseWire();
            return;
        }
    }

    QMainWindow::keyPressEvent(event);
}

void Window::on_fullscreen() {
    if (!this->isFullScreen()) {
        this->showFullScreen();
    } else {
        this->showNormal();
    }
}

int Window::getCurrentShader() {
    int shadeNumber = dm_acts.size();
    int current = 0;
    for (int i=0; i<shadeNumber;i++) {
        if (dm_acts.at(i)->isChecked()) {
            current = i;
            break;
        }
    }
    return current;
}

void Window::cycleShader(bool up) {
    int current = getCurrentShader();
    int updown = up ? 1 : -1;
    int nextS = (current + updown) % dm_acts.size();
    nextS = nextS < 0 ? dm_acts.size() - 1 : nextS;
    dm_acts.at(nextS)->setChecked(true);
    on_drawMode(dm_acts.at(nextS));
}


// Resize the widget giving the canvas dimension
// Useful for screenshot of given size.
void Window::setCanvasSize(int w, int h) {
    if (this->isFullScreen()) {
        fullscreen_action->toggle();
    }
    int dw = this->size().width() - canvas->size().width();
    int dh = this->size().height() - canvas->size().height();
    this->resize(w + dw, h + dh);
}

void Window::setViewportSize(QAction* act) {
    QString t = act->text();
    QRegExp rx = QRegExp("^\\s*(\\d+).+(\\d+).*");
    rx.indexIn(t);
    QStringList desc = rx.capturedTexts();
    int w = desc.at(1).toInt();
    int h = desc.at(2).toInt();
    setCanvasSize(w, h);
}

void Window::on_help() {
    //qDebug() << "help!";
    QMessageBox* helpWin = new QMessageBox(QMessageBox::NoIcon,"Help","",QMessageBox::Ok,this,Qt::Dialog);
    helpWin->setText(""
                     "<h2>Help</h2>"
                     "<h3>Shortcuts</h3>"
                     "<ul>"
                     "<li><b>H</b> : Display this help message"
                     "<li><b>Q</b> : Quit"
                     "<li><b>O</b> : Open"
                     "<li><b>R</b> : Reload the file"
                     "<li><b>P</b> : Draw Mode Settings for current shader (if available)"
                     "<li><b>A</b> : Draw Axes (and some informations)"
                     "<li><b>M</b> : Show/Hide Menu (and Toolbar as well)"
                     "<li><b>S</b> : Save Screenshot"
                     "<li><b>F</b> : Toggle Fullscreen"
                     "<li><b>W</b> : Toggle Wireframe on top of shader (if available)"
                     "<li><b>C</b> : Center View"
                     "<li><b>0-6</b> : Apply Default,Top,Bottom,Front,Rear,Left,Right view"
                     "<li><b>Left Arrow</b> : load previous stl file"
                     "<li><b>Right Arrow</b> : load next stl file"
                     "<li><b>Up Arrow</b> : use next shader"
                     "<li><b>Down Arrow</b> : use previous shader"
                     "</ul>"
                     );
    helpWin->show();

}

void Window::on_centerView() {
    canvas->recenterView();
    canvas->update();
}

void Window::onApplyView(QAction* act) {
    QString t = act->text();
    QRegExp rx = QRegExp("^\\s*(\\S+)\\s+.*$");
    rx.indexIn(t);
    QStringList desc = rx.capturedTexts();
    QString name = desc.at(1);

    if (name == "Default") {
        for ( QAction* a : defaultViewAction->actions()) {
            if (a->isChecked()) {
                canvas->applyRotation(a->text());
                break;
            }
        }
    } else {
        canvas->applyRotation(name);
    }
    canvas->update();
}


void Window::onSpeedMouseButton() {
    // toggle
    if (speedMouseDialog->isVisible()) {
        speedMouseDialog->hide();
        return;
    }
    // get button geometry and position
    int buttonWidth = speedMouseButton->geometry().width();
    int buttonHeight = speedMouseButton->geometry().height();
    QPoint dialogPos = speedMouseButton->mapToGlobal(QPoint(0,0));
    // dialoPos is now upper left corner of speedMouseButton in global coordinates
    // modify it
    dialogPos.setX(dialogPos.x()+buttonWidth);
    dialogPos.setY(dialogPos.y()+buttonHeight/2);
    // show dialog before getting geometry
    speedMouseDialog->show();
    int dialogWidth = speedMouseDialog->geometry().width();
    int dialogHeight = speedMouseDialog->geometry().height();
    speedMouseDialog->setGeometry(dialogPos.x(),dialogPos.y(),dialogWidth,dialogHeight);
}

void Window::onMsaaAction(QAction* act) {
    int mssaSetting = act->data().toInt();
    canvas->setMsaa(act->data().toInt());
    QString msaaSettingString = mssaSetting == -1 ? QString("Off\n") : QString("%1x\n").arg(mssaSetting);
    QMessageBox::information(this,"Anti-Aliasing",QString("Anti-Aliasing setting set to : ")
                                                   + msaaSettingString +
                                                    QString("It will take effect on next restart."));
}
