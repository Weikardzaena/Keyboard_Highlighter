#include "mainWindow.hpp"

using namespace std;

mainWindow::mainWindow(QWidget *parent) :
    QMainWindow(parent),
    initFailed(false)
{
    QLibrary user32("User32.dll");
    user32.load();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout() ), this, SLOT(updateInputState() ) );

    /* If initialization fails, return before we setScene.
     * Otherwize we get read access violations */
    if (!initVars()) {
        initFailed = true;
        return;
    }

    keyboardView->setScene(keyboardScene);
}


void mainWindow::beginLoop()
{
    timer->start(10);
}

void mainWindow::initScene(QRect *rect)
{
    keyboardScene = new QGraphicsScene(*rect);
    keyboardScene->addPixmap(*keyboardBackground);

    for(int i = 0; i < 254; ++i)
    {
        switch(i+1) /* Virtual Key codes start at 1 */
        {
        case 0x08: /* VK_BACK */
            keyHighlights[i] = keyboardScene->addPixmap(*backspaceKeyPixmap);
            break;
        case 0x09: /* VK_TAB */
            keyHighlights[i] = keyboardScene->addPixmap(*tabKeyPixmap);
            break;
        case 0x0D: /* VK_RETURN */
            keyHighlights[i] = keyboardScene->addPixmap(*enterKeyPixmap);
            break;
        case 0x14: /* VK_CAPITAL */
            keyHighlights[i] = keyboardScene->addPixmap(*capsKeyPixmap);
            break;
        case 0x20: /* VK_SPACE */
            keyHighlights[i] = keyboardScene->addPixmap(*spaceKeyPixmap);
            break;
        case 0x5B: /* VK_LWIN */
            keyHighlights[i] = keyboardScene->addPixmap(*bottomRowKeyPixmap);
            break;
        case 0x5C: /* VK_RWIN */
            keyHighlights[i] = keyboardScene->addPixmap(*bottomRowKeyPixmap);
            break;
        case 0x5D: /* VK_APPS */
            keyHighlights[i] = keyboardScene->addPixmap(*bottomRowKeyPixmap);
            break;
        case 0x60: /* VK_NUMPAD0 */
            keyHighlights[i] = keyboardScene->addPixmap(*zeroKeyPixmap);
            break;
        case 0x6B: /* VK_ADD */
            keyHighlights[i] = keyboardScene->addPixmap(*plusKeyRightPixmap);
            break;
        case 0xA0: /* VK_LSHIFT */
            keyHighlights[i] = keyboardScene->addPixmap(*shiftKeyLeftPixmap);
            break;
        case 0xA1: /* VK_RSHIFT */
            keyHighlights[i] = keyboardScene->addPixmap(*shiftKeyRightPixmap);
            break;
        case 0xA2: /* VK_LCONTROL */
            keyHighlights[i] = keyboardScene->addPixmap(*bottomRowKeyPixmap);
            break;
        case 0xA3: /* VK_RCONTROL */
            keyHighlights[i] = keyboardScene->addPixmap(*bottomRowKeyPixmap);
            break;
        case 0xA4: /* VK_LMENU */
            keyHighlights[i] = keyboardScene->addPixmap(*bottomRowKeyPixmap);
            break;
        case 0xA5: /* VK_RMENU */
            keyHighlights[i] = keyboardScene->addPixmap(*bottomRowKeyPixmap);
            break;
        case 0xDC: /* VK_OEM5 (the "\|" key on US standard keyboards) */
            keyHighlights[i] = keyboardScene->addPixmap(*pipeKeyPixmap);
            break;

        default:
            keyHighlights[i] = keyboardScene->addPixmap(*squareKeyPixmap);
            break;
        }
        keyHighlights[i]->setPos(pixmapPosData.at(2*i),pixmapPosData.at(2*i+1));
        keyHighlights[i]->setVisible(false);
    }
}

bool mainWindow::initVars()
{
    if (!loadPixmaps())
        return false;

    QRect baseImageRect = keyboardBackground->rect();
    this->setFixedSize(baseImageRect.size());
    this->move(150,300);

    if(!loadPixmapPosData())
        return false;

    initScene(&baseImageRect);
    initView(&baseImageRect);

    return true;
}

void mainWindow::initView(QRect *rect)
{
    keyboardView = new QGraphicsView(this);
    keyboardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    keyboardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    keyboardView->setFrameStyle(QFrame::NoFrame);
    keyboardView->setFixedSize(rect->size());
}

bool mainWindow::loadPixmaps()
{
    keyboardBackground      = new QPixmap();
    backspaceKeyPixmap      = new QPixmap();
    bottomRowKeyPixmap      = new QPixmap();
    capsKeyPixmap           = new QPixmap();
    enterKeyPixmap          = new QPixmap();
    pipeKeyPixmap           = new QPixmap();
    plusKeyRightPixmap      = new QPixmap();
    shiftKeyLeftPixmap      = new QPixmap();
    shiftKeyRightPixmap     = new QPixmap();
    spaceKeyPixmap          = new QPixmap();
    squareKeyPixmap         = new QPixmap();
    tabKeyPixmap            = new QPixmap();
    zeroKeyPixmap           = new QPixmap();

    if (!keyboardBackground->load("..\\var\\KeyboardBase.png"))
        return false;
    if (!backspaceKeyPixmap->load("..\\var\\BackspaceKey.png"))
        return false;
    if (!bottomRowKeyPixmap->load("..\\var\\BottomRowKey.png"))
        return false;
    if (!capsKeyPixmap->load("..\\var\\CapsKey.png"))
        return false;
    if (!enterKeyPixmap->load("..\\var\\EnterKey.png"))
        return false;
    if (!pipeKeyPixmap->load("..\\var\\PipeKey.png"))
        return false;
    if (!plusKeyRightPixmap->load("..\\var\\PlusKeyRight.png"))
        return false;
    if (!shiftKeyLeftPixmap->load("..\\var\\ShiftKeyLeft.png"))
        return false;
    if (!shiftKeyRightPixmap->load("..\\var\\ShiftKeyRight.png"))
        return false;
    if (!spaceKeyPixmap->load("..\\var\\SpaceKey.png"))
        return false;
    if (!squareKeyPixmap->load("..\\var\\SquareKey.png"))
        return false;
    if (!tabKeyPixmap->load("..\\var\\TabKey.png"))
        return false;
    if (!zeroKeyPixmap->load("..\\var\\ZeroKey.png"))
        return false;

    return true;
}

bool mainWindow::loadPixmapPosData()
{
    float number;
    int j = 0;
    streampos length;
    ifstream file;

    file.open("..\\var\\keyboardPosData.bin", ios::in | ios::binary);
    if(!file.is_open())
        return false;

    file.seekg(0,ios::end);
    length = file.tellg();
    file.seekg(0,ios::beg);

    while(j < length)
    {
        j += sizeof(number);
        file.read(reinterpret_cast<char *>(&number),sizeof(number));
        pixmapPosData.push_back(number);
    }

    file.close();
    return true;
}

void mainWindow::updateInputState()
{
    /* This is the only safe way to kill the window because
     * this way Qt deals with the window destructors and we
     * don't have to worry about freeing memory
     */
    if(initFailed)
    {
        QMessageBox errorWindow;
        errorWindow.setIcon(QMessageBox::Critical);
        errorWindow.setText("Error!");
        errorWindow.setInformativeText("Window Initialization Failed! Quitting.");
        errorWindow.exec();
        exit(1);
    }

    for (int i = 1; i < 255; ++i)
    {
        keyboardArray[i] = (GetAsyncKeyState(i) & 0x8000) >> 15;
        if (keyboardArray[i])
            keyHighlights[i-1]->setVisible(true);
        else
            keyHighlights[i-1]->setVisible(false);
    }
}
void mainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Home) {
        Qt::WindowFlags flags = this->windowFlags();
        if (flags.testFlag(Qt::WindowStaysOnTopHint))
            flags &= ~Qt::WindowStaysOnTopHint;
        else
            flags |= Qt::WindowStaysOnTopHint;
        this->setWindowFlags(flags);
        this->show();
    }
}
