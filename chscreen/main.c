#include <ApplicationServices/ApplicationServices.h>
#include <stdio.h>

/**
 * マルチディスプレイ環境下で、指定したディスプレイの中央にマウスカーソルを移動する。
 * 左右にマウスカーソルを移動するのが辛いので...。
 *
 * ショートカッキーに割り当てて（Automator経由で）起動すると便利です。
 *
 * 使い方: 0番目のディスプレイにカーソル移動
 *   chscreen 0
 *
 * 使い方: 1番目のディスプレイにカーソル移動
 *   chscreen 1
 *
 * 使い方: 前のディスプレイにカーソル移動
 *   chscreen -1
 *
 * 使い方: 次のディスプレイにカーソル移動
 *   chscreen -2
 *
 * コンパイル:
 *   clang -O2 -framework ApplicationServices ./main.c -o chscreen
 */
int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("An argument for the display number is required.\n");
        exit(EXIT_FAILURE);
    }

    int numDisplay = atoi(argv[1]);

    // 現在のカーソル位置の取得
    CGEventRef event = CGEventCreate(NULL);
    CGPoint currentPt = CGEventGetLocation(event);
    CFRelease(event);

    // ディスプレイのリストを取得
    CGDisplayCount screenCount;
    CGGetOnlineDisplayList(INT_MAX, NULL, &screenCount);

    CGDirectDisplayID *pScreenList = malloc(sizeof(CGDirectDisplayID) * screenCount);
    CGGetOnlineDisplayList(INT_MAX, pScreenList, &screenCount);

    // 入力チェック
    if (numDisplay >= 0)
    {
        if (numDisplay >= screenCount)
        {
            printf("The valid numbers range from 0 to %d.\n", screenCount - 1);
            free(pScreenList);
            exit(EXIT_FAILURE);
        }
    }
    else if (numDisplay != -1 && numDisplay != -2)
    {
        printf("The negative numbers that can be specified as arguments are either -1 or -2.\n");
        free(pScreenList);
        exit(EXIT_FAILURE);
    }

    // 現在のマウスカーソルはどのディスプレイにいる？
    // 該当するディスプレイのインデックスを取得。
    int curScreenIdx = 0;
    {
        for (int i = 0; i < screenCount; i++)
        {
            CGDirectDisplayID displayId = pScreenList[i];
            CGRect rect = CGDisplayBounds(displayId);
            if (currentPt.x >= rect.origin.x && currentPt.x <= rect.origin.x + rect.size.width &&
                currentPt.y >= rect.origin.y && currentPt.y <= rect.origin.y + rect.size.height)
            {
                curScreenIdx = i;
                break;
            }
        }
    }

    // 負数の場合は移動先のディスプレイを前後に移動。
    // 整数の場合はインデックスに対応するディスプレイに移動。
    switch (numDisplay)
    {
    case -1: // 前
        curScreenIdx--;
        break;
    case -2: // 次
        curScreenIdx++;
        break;
    default: // ディスプレイ指定
        curScreenIdx = numDisplay;
        break;
    }

    // 先端・終端を超えたらループさせる。
    if (curScreenIdx < 0)
    {
        // ループさせるため終端へ。
        curScreenIdx = screenCount - 1;
    }
    else if (curScreenIdx >= screenCount)
    {
        // ループさせるため先端へ。
        curScreenIdx = 0;
    }

    // ディスプレイの幅を取得して、カーソル位置を中央に。
    CGDirectDisplayID screenId = pScreenList[curScreenIdx];
    int w = CGDisplayPixelsWide(screenId);
    int h = CGDisplayPixelsHigh(screenId);

    CGPoint pt;
    pt.x = w / 2;
    pt.y = h / 2;
    CGDisplayMoveCursorToPoint(screenId, pt);

    // 後処理（繰り返し動作しないのだから、解放しなくても良いのだろうけど...）
    free(pScreenList);

    return EXIT_SUCCESS;
}