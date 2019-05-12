using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class MainGame : MonoBehaviour {

    public enum GravityDirection
    {
        GRAVITY_DOWN, GRAVITY_LEFT, GRAVITY_UP, GRAVITY_RIGHT, GRAVITY_OUTWARDS
    }

    // Game Init
    private GameObject objBoardArray;      // 게임 보드판 오브젝트
    private GameObject objBoardFrame;      // 게임 프레임 오브젝트
    private SpriteRenderer frameRenderer;  // 프레임 오브젝트 스프라이트 렌더러
    private Color frameColor;              // 프레임 오브젝트 색상
    private GameObject objLinkLines;       // 링크 라인 오브젝트
    private GameObject objSounds;          // 사운드 그룹 오브젝트

    // Game UI Init
    private GameObject uiobjUI;                // UI
    private GameObject uiobjOptionBtnGrp;      // 옵션 버튼 그룹
    private GameObject uiobjPauseBoard;        // 일시정지 UI오브젝트
    private GameObject uiobjScoreBoard;        // 스코어보드 UI오브젝트
    private GameObject uiobjGravityImage;      // 중력 이미지 UI오브젝트
    private GameObject uiobjNoMoreLinkCntText; // NoMoreMove 버튼 텍스트 UI오브젝트

    // Game Logic
    private bool bGameOver;                     // 게임오버 여부
    private int iLastOctave;                    // 마지막으로 재생된 음계
    private bool bScrClicked;                   // 스크린 터치됨 여부
    private DotObject[,] boardData;             // 보드 데이터
    private List<DotObject> selectedDotList;    // 선택된 Dot을 담은 리스트

    // Game Optional Logic
    public GameMode gameMode;                   // 게임 모드 스크립트
    public bool bPaused;                        // 일시 정지 여부
    public float fPlaytime;                     // 게임 플레이 시간
    public GravityDirection enGravityDir;       // 중력 방향
    public int iNoMoreMoveCnt;                  // NoMoreMove 카운터
    public int iScore;                          // 플레이어 점수
    public int iMaxLink, iLinkCnt;              // 최대 링크, 현재 링크
    public int iScoreBase;                      // 기본 점수 상승률

    // Game Event
    public delegate void EvtOnScoreUpdate();
    public EvtOnScoreUpdate OnScoreUpdate;

	// Use this for initialization
	void Start ()
    {
        // 게임 오브젝트 참조 초기화
        objBoardArray = transform.FindChild("Board Array").gameObject;
        objBoardFrame = transform.FindChild("Board Frame").gameObject;
        frameRenderer = objBoardFrame.GetComponent<SpriteRenderer>();
        objLinkLines = transform.FindChild("Link Lines").gameObject;
        objSounds = transform.FindChild("Sounds").gameObject;
        uiobjUI = GameObject.Find("UI");
        uiobjScoreBoard = uiobjUI.transform.FindChild("Score Board").gameObject;
        uiobjGravityImage = uiobjUI.transform.FindChild("Setting Board").FindChild("Gravity Button").FindChild("Gravity Image").gameObject;
        uiobjNoMoreLinkCntText = uiobjUI.transform.FindChild("Setting Board").FindChild("Dot Init Button").FindChild("Dot Init Amount Text").gameObject;
        uiobjOptionBtnGrp = uiobjUI.transform.FindChild("Option Buttons").gameObject;
        uiobjPauseBoard = uiobjUI.transform.FindChild("Paused Board").gameObject;

        // 게임 모드 설정
        switch (PlayerData.SelectedGameMode)
        {
            default:
            case PlayerData.GameModes.MODE_NORMAL:
                gameMode = gameObject.AddComponent<ModeNormal>();
                break;
            case PlayerData.GameModes.MODE_INFINITE:
                gameMode = gameObject.AddComponent<ModeInfinite>();
                break;
            case PlayerData.GameModes.MODE_MEDITATION:
                gameMode = gameObject.AddComponent<ModeMeditation>();
                break;
        }

        // 게임 초기화
        InitializeGame();

        // 보드 초기화
        InitializeBoard();
    }

    // Update is called once per frame
    void Update()
    {
        if (!bPaused && !bGameOver)
        {
            // 시간 증가
            fPlaytime += Time.deltaTime;

            // 스크린 터치 업데이트
            ScrTouchUpdate();

            // 스크린 드래그 업데이트
            ScrDragUpdate();

            // 보드 프레임 색상 업데이트
            UpdateFrameColor();
        }
    }

    // 게임 초기화
    public void InitializeGame()
    {
        Time.timeScale = 1.000f;
        bPaused = false;
        bGameOver = false;
        iLastOctave = 0;
        bScrClicked = false;
        selectedDotList = new List<DotObject>();

        frameColor = new Color(80.00f / 255.00f, 20.00f / 255.00f, 20.00f / 255.00f);
        enGravityDir = GravityDirection.GRAVITY_DOWN;
        iNoMoreMoveCnt = 1;
        iScore = 0;
        iMaxLink = 0;
        iLinkCnt = 0;
        iScoreBase = 35;
    }

    // 보드 초기화
    public void InitializeBoard()
    {
        int iBoardRow = GameData.iBoardRow, iBoardCol = GameData.iBoardCol;

        // 생성된 Dot이 있다면 제거
        if (objBoardArray.transform.childCount > 0)
        {
            for (int i = 0; i < objBoardArray.transform.childCount; ++i)
            {
                Destroy(objBoardArray.transform.GetChild(i).gameObject);
            }
        }

        // 생성된 LinkLine이 있다면 제거
        if (objLinkLines.transform.childCount > 0)
        {
            RemoveLinkLineAll();
        }

        // 보드 데이터 생성
        boardData = new DotObject[iBoardRow, iBoardCol];

        Vector3 pos = Vector3.zero;                         // Dot의 위치

        // 좌상 우하 순서로 Dot 생성
        for (int i = 0; i < iBoardRow; ++i)
        {
            for (int j = 0; j < iBoardCol; ++j)
            {
                pos = new Vector3(-GameData.fBoardHalfWidth + j * GameData.fWidthDist, GameData.fBoardHalfHeight - i * GameData.fHeightDist, 0.00f);
                DotObject.CreateDot(pos, i, j, true, objBoardArray.transform, boardData);
            }
        }
    }

    // 스크린 클릭
    public void ScrTouchUpdate()
    {
        // 클릭
        if (Input.GetMouseButtonDown(0))
        {
            bScrClicked = true;

            // 클릭된 지점의 점 선택
            SelectDot(GetDotInTouchPos());
        }

        // 클릭 취소
        if (Input.GetMouseButtonUp(0))
        {
            bScrClicked = false;

            // 점수 연산
            UpdateScore();

            // LinkLine 제거
            RemoveLinkLineAll();

            // Dot링크 처리
            UpdateLink();
        }
    }

    // 스크린 드래그
    public void ScrDragUpdate()
    {
        if (bScrClicked)
        {
            DotObject dot = GetDotInTouchPos();
            
            if (CheckDot(dot))
            {
                // 드래그중인 지점의 점 업데이트
                SelectDot(dot);

                // 링크 라인 생성
                CreateLinkLine(dot);
            }
        }
    }

    // 터치된 지점의 Dot을 반환하는 메서드
    private DotObject GetDotInTouchPos()
    {
        RaycastHit hitObject; // 클릭된 오브젝트를 저장할 변수

        if (Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out hitObject, Camera.main.farClipPlane))
        {
            if (hitObject.transform.tag == "Dot")
            {
                return hitObject.transform.GetComponent<DotObject>();
            }
        }

        return null;
    }

    // Dot을 선택 하는 메서드
    private void SelectDot(DotObject dot)
    {
        if (dot != null)
        {
            dot.SelectDot();
            selectedDotList.Add(dot);
        }
    }

    // 올바른 조건의 Dot인지 확인하는 메서드
    private bool CheckDot(DotObject dot)
    {
        // 현재 선택된 Dot이 있는지 확인
        if (dot == null) return false;

        // 기존에 선택된 Dot이 있는지 확인
        int iDotCount = selectedDotList.Count;
        if (iDotCount <= 0) return false;

        // 선택된 Dot이 Normal상태의 Dot인지 확인
        if (dot.enState != DotObject.DotState.DOT_STATE_NORMAL) return false;

        // 기존에 선택된 Dot과 십자(+)로 인접한 Dot인지 확인
        DotObject lastDot = selectedDotList[iDotCount - 1];
        int iCol = lastDot.iCol, iRow = lastDot.iRow;
        bool bDotAdjacent = false;

        if (iRow - 1 > -1) // 기존에 선택된 Dot위치에서 상하좌우를 비교하여 새로 선택된 Dot이 인접한지 확인
        {
            if (boardData[iRow - 1, iCol] == dot) bDotAdjacent = true;    // 상단
        }
        if (iRow + 1 < GameData.iBoardRow)
        {
            if (boardData[iRow + 1, iCol] == dot) bDotAdjacent = true;    // 하단
        }
        if (iCol - 1 > -1)
        {
            if (boardData[iRow, iCol - 1] == dot) bDotAdjacent = true;    // 좌측
        }
        if (iCol + 1 < GameData.iBoardCol)
        {
            if (boardData[iRow, iCol + 1] == dot) bDotAdjacent = true;    // 우측
        }
        if (!bDotAdjacent) return false;

        // 색상 비교
        if (dot.enColor != DotObject.DotColor.DOT_COLOR_RAINBOW && lastDot.enColor != DotObject.DotColor.DOT_COLOR_RAINBOW)
        {
            if (lastDot.enColor != dot.enColor) return false;
        }

        // 모든 조건을 충족한 경우 true반환
        return true;
    }

    // Dot링크 처리 메서드
    private void UpdateLink()
    {
        if (selectedDotList.Count > 1)
        {
            for (int i = 0; i < selectedDotList.Count; ++i)
            {
                DotObject linkedDot = selectedDotList[i];
                linkedDot.enState = DotObject.DotState.DOT_STATE_DELETING;
                linkedDot.transform.position += Vector3.forward;
                boardData[linkedDot.iRow, linkedDot.iCol] = null;
            }
            selectedDotList.Clear();
            UpdateDataBoard();
            FillDataBoard();
        }
        else if (selectedDotList.Count == 1)
        {
            selectedDotList[0].enState = DotObject.DotState.DOT_STATE_NORMAL;
            selectedDotList.Clear();
        }
    }

    // Databoard 업데이트
    private void UpdateDataBoard()
    {
        switch (enGravityDir)
        {
            default:
            case GravityDirection.GRAVITY_DOWN:
                for (int col = 0; col < GameData.iBoardCol; ++col)
                {
                    for (int row = GameData.iBoardRow - 1; row > 0; --row)
                    {
                        if (boardData[row, col] == null)
                        {
                            for (int i = row - 1; i >= 0; --i)
                            {
                                if (boardData[i, col] != null)
                                {
                                    boardData[row, col] = boardData[i, col];
                                    boardData[i, col] = null;
                                    boardData[row, col].iCol = col;
                                    boardData[row, col].iRow = row;
                                    boardData[row, col].enState = DotObject.DotState.DOT_STATE_MOVING;
                                    break;
                                }
                            }
                        }
                    }    
                }
                break;
            case GravityDirection.GRAVITY_UP:
                for (int col = 0; col < GameData.iBoardCol; ++col)
                {
                    for (int row = 0; row < GameData.iBoardRow - 1; ++row)
                    {
                        if (boardData[row, col] == null)
                        {
                            for (int i = row + 1; i < GameData.iBoardRow; ++i)
                            {
                                if (boardData[i, col] != null)
                                {
                                    boardData[row, col] = boardData[i, col];
                                    boardData[i, col] = null;
                                    boardData[row, col].iCol = col;
                                    boardData[row, col].iRow = row;
                                    boardData[row, col].enState = DotObject.DotState.DOT_STATE_MOVING;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
            case GravityDirection.GRAVITY_LEFT:
                for (int row = 0; row < GameData.iBoardRow; ++row)
                { 
                    for (int col = 0; col < GameData.iBoardCol - 1; ++col)
                    {
                        if (boardData[row, col] == null)
                        {
                            for (int i = col + 1; i < GameData.iBoardCol; ++i)
                            {
                                if (boardData[row, i] != null)
                                {
                                    boardData[row, col] = boardData[row, i];
                                    boardData[row, i] = null;
                                    boardData[row, col].iCol = col;
                                    boardData[row, col].iRow = row;
                                    boardData[row, col].enState = DotObject.DotState.DOT_STATE_MOVING;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
            case GravityDirection.GRAVITY_RIGHT:
                for (int row = 0; row < GameData.iBoardRow; ++row)
                {
                    for (int col = GameData.iBoardCol - 1; col > 0; --col)
                    {
                        if (boardData[row, col] == null)
                        {
                            for (int i = col - 1; i >= 0; --i)
                            {
                                if (boardData[row, i] != null)
                                {
                                    boardData[row, col] = boardData[row, i];
                                    boardData[row, i] = null;
                                    boardData[row, col].iCol = col;
                                    boardData[row, col].iRow = row;
                                    boardData[row, col].enState = DotObject.DotState.DOT_STATE_MOVING;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
            case GravityDirection.GRAVITY_OUTWARDS:

                break;
        }
    }

    // Databoard의 빈 공간 채우기
    private void FillDataBoard()
    {
        Vector3 pos;

        for (int i = 0; i < GameData.iBoardRow; ++i)
        {
            for (int j = 0; j < GameData.iBoardCol; ++j)
            {
                if (boardData[i,j] == null)
                {
                    switch (enGravityDir)
                    {
                        default:
                        case GravityDirection.GRAVITY_DOWN:
                            pos = new Vector3(-GameData.fBoardHalfWidth + j * GameData.fWidthDist, GameData.fBoardHeight - i * GameData.fHeightDist, 0.00f);
                            break;
                        case GravityDirection.GRAVITY_UP:
                            pos = new Vector3(-GameData.fBoardHalfWidth + j * GameData.fWidthDist, -GameData.fBoardHeight - i * GameData.fHeightDist, 0.00f);
                            break;
                        case GravityDirection.GRAVITY_LEFT:
                            pos = new Vector3(GameData.fBoardWidth + j * GameData.fWidthDist, GameData.fBoardHalfHeight - i * GameData.fHeightDist, 0.00f);
                            break;
                        case GravityDirection.GRAVITY_RIGHT:
                            pos = new Vector3(-GameData.fBoardWidth + j * GameData.fWidthDist, GameData.fBoardHalfHeight - i * GameData.fHeightDist, 0.00f);
                            break;
                        case GravityDirection.GRAVITY_OUTWARDS:
                            pos = new Vector3(-GameData.fBoardHalfWidth + j * GameData.fWidthDist, GameData.fBoardHalfHeight - i * GameData.fHeightDist, 0.00f);
                            break;
                    }
                    DotObject.CreateDot(pos, i, j, true, objBoardArray.transform, boardData);
                }
            }
        }
    }

    // 보드 프레임 색상 업데이트
    private void UpdateFrameColor()
    {
        if (selectedDotList.Count > 0)
        {
            frameRenderer.color = selectedDotList[selectedDotList.Count - 1].GetDotColor();
        }
        else
        {
            float fH, fS, fV;
            Color.RGBToHSV(frameColor, out fH, out fS, out fV);
            frameRenderer.color = Color.HSVToRGB(fH, fS - 41.00f / 255.00f, fV + 40.00f / 255.00f);
        }
    }

    // 링크 라인 생성
    private void CreateLinkLine(DotObject dot)
    {
        // 생성
        GameObject objLine = Instantiate(GameData.pfabLine, dot.transform.position, Quaternion.identity, objLinkLines.transform) as GameObject;
        LineRenderer lineRenderer = objLine.GetComponent<LineRenderer>();
        DotObject lastDot = selectedDotList[selectedDotList.Count - 2];
        Vector3 posOrigin = new Vector3(lastDot.transform.position.x, lastDot.transform.position.y, 0.20f),
                posFinal  = new Vector3(dot.transform.position.x, dot.transform.position.y, 0.20f);
        Vector3[] posAry = { posOrigin, posFinal };
        Color dotColor = dot.GetDotColor();

        if (dot.enColor == DotObject.DotColor.DOT_COLOR_RAINBOW) dotColor = lastDot.GetDotColor();

        lineRenderer.SetPositions(posAry);
        lineRenderer.startColor = dotColor;
        lineRenderer.endColor = dotColor;

        // 사운드 재생
        GameObject objSound = Instantiate(GameData.pfabSoundEft, dot.transform.position, Quaternion.identity, objSounds.transform) as GameObject;
        AudioSource sound = objSound.GetComponent<AudioSource>();
        sound.volume = (PlayerData.iSoundMultiply / 100.00f);
        sound.PlayOneShot(GameData.OctaveAudio[++iLastOctave % 7]);
        sound.pitch = (((objLinkLines.transform.childCount - 1) / 7) * 0.10f) + 1.00f;
        Destroy(objSound, 0.65f);
    }

    // 모든 링크 라인 제거
    private void RemoveLinkLineAll()
    {
        for (int i = 0; i < objLinkLines.transform.childCount; ++i)
        {
            Destroy(objLinkLines.transform.GetChild(i).gameObject);
        }
    }

    // 점수 연산
    public void UpdateScore()
    {
        if (selectedDotList.Count < 2) return;

        // 연산
        int iLinked = selectedDotList.Count - 1;
        int iGottenScore = (iLinked * iLinked) * iScoreBase;
        iScore += iGottenScore;
        iLinkCnt += iLinked;
        if (iMaxLink < iLinked) iMaxLink = iLinked;

        // UI 수정
        Transform scoreTextTrans = uiobjScoreBoard.transform.FindChild("Score Text");
        scoreTextTrans.GetComponent<Text>().text = iScore.ToString();
        scoreTextTrans.GetComponent<ScoreText>().DoDynamicEffect();
        uiobjScoreBoard.transform.FindChild("Max Link Text").GetComponent<Text>().text = "Max Link : " + iMaxLink.ToString();
        uiobjScoreBoard.transform.FindChild("Total Link Text").GetComponent<Text>().text = "Total Link : " + iLinkCnt.ToString();

        // UI 생성
        if (iGottenScore > 500)
        {
            GameObject objDynamicScoreText = Instantiate(GameData.pfabScoreTextUI, Camera.main.WorldToScreenPoint(
                                                         selectedDotList[iLinked].transform.position), Quaternion.identity, uiobjUI.transform);
            Text dsText = objDynamicScoreText.GetComponent<Text>();
            string strTail = "";
            float fDuration = 0.75f;

            if (iGottenScore > 10000)
            {
                dsText.fontSize = 70;
                strTail = "!!!";
                fDuration = 1.50f;
            }
            else if (iGottenScore > 7000)
            {
                dsText.fontSize = 65;
                strTail = "!!";
                fDuration = 1.25f;
            }
            else if (iGottenScore > 2000)
            {
                dsText.fontSize = 60;
                strTail = "!";
                fDuration = 1.00f;
            }
            else if (iGottenScore > 500)
            {
                dsText.fontSize = 50;
                strTail = "";
                fDuration = 0.75f;
            }

            dsText.text = "+" + iGottenScore + strTail;
            dsText.color = selectedDotList[iLinked].GetComponent<DotObject>().GetDotColor();
            objDynamicScoreText.GetComponent<DynamicText>().SetDynamicText(fDuration, Vector3.up);
        }

        // 이벤트 함수 수행
        if (OnScoreUpdate != null) OnScoreUpdate();

        // Normal Mode시 시간초 증가
        if (gameMode.GetType() == typeof(ModeNormal))
        {
            ModeNormal mode = gameMode as ModeNormal;
            mode.AddTimeLeft(iLinked);
        }
    }

    // 게임오버 실행
    public void DoGameover()
    {
        GameObject uiobjGameover = uiobjUI.transform.FindChild("Gameover Board").gameObject;

        // 최초 게임오버시
        if (uiobjGameover.activeSelf == false)
        {
            bGameOver = true;
            uiobjScoreBoard.SetActive(false);
            uiobjGameover.SetActive(true);
            uiobjGameover.GetComponent<GameoverBoard>().RenewBoard();
        }
    }

    // 중력 방향 변경
    public void UI_ChangeGravityDir()
    {
        // 방향 및 UI변경
        if (enGravityDir != GravityDirection.GRAVITY_OUTWARDS)
        {
            ++enGravityDir;

            if (enGravityDir == GravityDirection.GRAVITY_OUTWARDS)
            {
                uiobjGravityImage.GetComponent<RectTransform>().rotation.eulerAngles.Set(0.00f, 0.00f, 0.00f);
                uiobjGravityImage.GetComponent<Image>().sprite = GameData.SpriteGravity_B;

            }
            else
            {
                uiobjGravityImage.GetComponent<RectTransform>().rotation = Quaternion.AngleAxis((float)enGravityDir * 90.00f, Vector3.back);
            }
        }
        else
        {
            enGravityDir = GravityDirection.GRAVITY_DOWN;
            uiobjGravityImage.GetComponent<RectTransform>().rotation = Quaternion.identity;
            uiobjGravityImage.GetComponent<Image>().sprite = GameData.SpriteGravity_A;
        }
    }

    // 모든 Dot재배치
    public void UI_NoMoreMoveInit()
    {
        if (iNoMoreMoveCnt > 0) --iNoMoreMoveCnt;

        UI_UpdateNomoreMove();

        if (iNoMoreMoveCnt > 0 || iNoMoreMoveCnt == -1) // (0이상 or INF)
        {    
            InitializeBoard();
        }
    }

    // 메인 화면으로 이동
    public void UI_GotoMain()
    {
        Time.timeScale = 1.00f;
        SceneManager.LoadScene("MainMenu");
    }

    // 재시작
    public void UI_Restart()
    {
        SceneManager.LoadScene("MainGame");
    }

    // No more move UI 갱신
    public void UI_UpdateNomoreMove()
    {
        if (iNoMoreMoveCnt != -1)
        {
            uiobjNoMoreLinkCntText.GetComponent<Text>().text = iNoMoreMoveCnt.ToString();
        }
        else
        {
            uiobjNoMoreLinkCntText.GetComponent<Text>().text = "∞";
        }
    }

    // 일시 정지 버튼
    public void UI_DoPause()
    {
        uiobjPauseBoard.SetActive(true);
        Time.timeScale = 0.00f;
    }

    // 개임 재게 버튼
    public void UI_DoResume()
    {
        uiobjPauseBoard.SetActive(false);
        Time.timeScale = 1.00f;
    }

    // 종료 버튼
    public void UI_Quit()
    {
        UI_GotoMain();
    }
}

