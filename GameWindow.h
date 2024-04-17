#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include "GameWidgets.h"

class GameWindow : public QWidget {
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = nullptr);

private:
    HighScoreWidget *highScoreWidget;
    ScoreWidget *scoreWidget;
    LevelWidget *levelWidget;
    LivesWidget *livesWidget;
    GameMapWidget *gameMapWidget;
    
    int readHighScore();
};

