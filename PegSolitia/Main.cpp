# include <Siv3D.hpp>
# include "PegSolitia.hpp"
# include "GameScore.hpp"

// シーンの名前
enum class State
{
	Title,
	Tutorial,
	Game,
	Result
};

// ゲームデータ
struct GameData
{
	//難易度
	PegSolitia::Difficulty diff;
	// ハイスコア
	s3d::Duration score;
};

// シーン管理クラス
using MyApp = SceneManager<State, GameData>;

// タイトルシーン
class Title : public MyApp::Scene
{
private:

	Rect m_tutorialButton = Rect(Arg::center = (PegSolitia::WindowSize / 2).movedBy(0, -100), 300, 60);
	Transition m_tutorialTransition = Transition(0.4s, 0.2s);

	Rect m_startButton[3] = { Rect(Arg::center = (PegSolitia::WindowSize / 2).movedBy(0, 0), 300, 60),
								Rect(Arg::center = (PegSolitia::WindowSize / 2).movedBy(0, 100), 300, 60),
								Rect(Arg::center = (PegSolitia::WindowSize / 2).movedBy(0, 200), 300, 60) };
	Transition m_startTransition[3] = { Transition(0.4s, 0.2s) ,Transition(0.4s, 0.2s) ,Transition(0.4s, 0.2s) };

	Rect m_exitButton = Rect(Arg::center = (PegSolitia::WindowSize / 2).movedBy(0, 300), 300, 60);
	Transition m_exitTransition = Transition(0.4s, 0.2s);
public:

	Title(const InitData& init)
		: IScene(init) {}

	void update() override
	{
		m_tutorialTransition.update(m_tutorialButton.mouseOver());
		for (int i = 0; i < 3; i++)
			m_startTransition[i].update(m_startButton[i].mouseOver());
		m_exitTransition.update(m_exitButton.mouseOver());

		if (m_tutorialButton.mouseOver() || m_exitButton.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		if (m_tutorialButton.leftClicked())
		{
			changeScene(State::Tutorial);
		}

		for (int i = 0; i < 3; i++) {
			if (m_startButton[i].mouseOver())
			{
				Cursor::RequestStyle(CursorStyle::Hand);
			}

			if (m_startButton[i].leftClicked())
			{
				getData().diff = (PegSolitia::Difficulty)i;
				changeScene(State::Game);
			}
		}

		if (m_exitButton.leftClicked())
		{
			System::Exit();
		}
	}

	void draw() const override
	{
		const String titleText = U"ペグソリティア";
		const Vec2 center((PegSolitia::WindowSize / 2).x, 120);
		FontAsset(U"Title")(titleText).drawAt(center.movedBy(4, 6), ColorF(0.0, 0.5));
		FontAsset(U"Title")(titleText).drawAt(center);

		m_tutorialButton.draw(ColorF(1.0, m_tutorialTransition.value())).drawFrame(2);
		for (int i = 0; i < 3; i++)
			m_startButton[i].draw(ColorF(1.0, m_startTransition[i].value())).drawFrame(2);
		m_exitButton.draw(ColorF(1.0, m_exitTransition.value())).drawFrame(2);

		//ここにモード選択とハイスコア表示。

		FontAsset(U"Menu")(U"あそびかた").drawAt(m_tutorialButton.center(), ColorF(0.25));
		FontAsset(U"Menu")(U"かんたん").drawAt(m_startButton[0].center(), ColorF(0.25));
		FontAsset(U"Menu")(U"ふつう").drawAt(m_startButton[1].center(), ColorF(0.25));
		FontAsset(U"Menu")(U"むずかしい").drawAt(m_startButton[2].center(), ColorF(0.25));
		FontAsset(U"Menu")(U"おわる").drawAt(m_exitButton.center(), ColorF(0.25));

		Rect(0, 500, PegSolitia::WindowSize.x, PegSolitia::WindowSize.y - 500)
			.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

		//const s3d::Duration highScore = this->getData().highScore;
		//FontAsset(U"Score")(U"High score: {}"_fmt(highScore)).drawAt(Vec2(WindowSize.y - 100, WindowSize.x - 100));
	}
};

//チュートリアル
class Tutorial : public MyApp::Scene
{

	//パラパラ漫画
	Array<TextureRegion> textures;
	Texture leftTexture;
	Texture rightTexture;
	Polygon rightPolygon;
	Polygon leftPolygon;

	Rect back;

	int page;

public:

	Tutorial(const InitData& init)
		: IScene(init)
	{
		Array<FilePath> texturePaths;
		texturePaths = FileSystem::DirectoryContents(U"tutorial");
		for (const auto& path : texturePaths) {
			textures << Texture(path).resized(500);
		}

		back = Rect(textures[0].regionAt(PegSolitia::WindowSize / 2));

		leftTexture = Texture(Emoji::CreateImage(U"◀"));
		rightTexture = Texture(Emoji::CreateImage(U"▶"));

		rightPolygon = Emoji::CreateImage(U"◀").alphaToPolygons()[0];
		leftPolygon = Emoji::CreateImage(U"▶").alphaToPolygons()[0];

		rightPolygon = rightPolygon.movedBy(PegSolitia::WindowSize.x / 10 - leftTexture.width() / 2, PegSolitia::WindowSize.y / 2 - leftTexture.height() / 2);
		leftPolygon = leftPolygon.movedBy(PegSolitia::WindowSize.x * 9 / 10 - rightTexture.width() / 2, PegSolitia::WindowSize.y / 2 - rightTexture.height() / 2);

		page = 0;
	}

	void update() override
	{
		//先に進むアイコン
		//戻るアイコン
		//タイトルへ戻るが一番下。
		if (SimpleGUI::ButtonAt(U"タイトル", Point(PegSolitia::WindowSize.x / 2, PegSolitia::WindowSize.y - 60))) {
			changeScene(State::Title);
		}

		textures[page].drawAt(PegSolitia::WindowSize / 2);

		if (leftPolygon.leftClicked()) {
			page = Min(page + 1, (int)textures.size() - 1);
		}
		if (rightPolygon.leftClicked()) {
			page = Max(page - 1, 0);
		}

	}

	void draw() const override
	{

		//現在の画像を中央に描画
		if (rightPolygon.mouseOver())
			leftTexture.drawAt(PegSolitia::WindowSize.x / 10, PegSolitia::WindowSize.y / 2, Palette::Gray);
		else
			leftTexture.drawAt(PegSolitia::WindowSize.x / 10, PegSolitia::WindowSize.y / 2);

		if (leftPolygon.mouseOver())
			rightTexture.drawAt(PegSolitia::WindowSize.x * 9 / 10, PegSolitia::WindowSize.y / 2, Palette::Gray);
		else
			rightTexture.drawAt(PegSolitia::WindowSize.x * 9 / 10, PegSolitia::WindowSize.y / 2);

		FontAsset(U"Menu")(U"{}/{}"_fmt(page + 1, textures.size())).drawAt(PegSolitia::WindowSize.x / 2, PegSolitia::WindowSize.y - 150, ColorF(0.25));

		back.drawFrame(10);
	}
};


// ゲームシーン
class Game : public MyApp::Scene
{
private:

	Circle BigCircle;
	Circle SmallCircle;


	PegSolitia::Board board;

	Stopwatch sw;

	bool isHinted;
	Line hintArrow;

public:

	Game(const InitData& init)
		: IScene(init)
	{
		BigCircle = Circle(Window::ClientCenter(), 400);
		SmallCircle = Circle(Window::ClientCenter(), 350);
		board = PegSolitia::Board(9, 9);
		switch (getData().diff) {
		case PegSolitia::Difficulty::Easy:
			board.randomInitEasy();
			break;
		case PegSolitia::Difficulty::Hard:
			board.randomInitHard();
			break;
		case PegSolitia::Difficulty::Hell:
			board.randomInitHell();
			break;
		}
		while (!board.canSolve())
			board.randomInit();
		sw.start();
		isHinted = false;
	}

	void update() override
	{
		bool isChange = board.updata();

		if (SimpleGUI::Button(U"ヒント、ペナルティ：１分", Point(50, PegSolitia::WindowSize.y - 60))) {
			hintArrow = board.hint();
			auto duration = sw.elapsed();
			duration += 1min;
			sw.set(duration);
			isHinted = true;
		}
		if (SimpleGUI::Button(U"もどす", Point(360, PegSolitia::WindowSize.y - 60))) {
			if (board.back()) {
				auto duration = sw.elapsed();
				//duration += 1min;
				sw.set(duration);
				isHinted = false;
			}
		}
		if (SimpleGUI::Button(U"すすめる", Point(490, PegSolitia::WindowSize.y - 60))) {
			board.advance();
			isHinted = false;
		}
		if (SimpleGUI::Button(U"リセット", Point(630, PegSolitia::WindowSize.y - 60))) {
			board.reset();
			sw.restart();
			isHinted = false;
		}
		if (SimpleGUI::Button(U"タイトル", Point(760, PegSolitia::WindowSize.y - 60))) {
			board.reset();
			sw.restart();
			changeScene(State::Title);
		}

		if (isChange)
			isHinted = false;

		if (board.checkSolved()) {

			this->getData().score = sw.elapsed();

			changeScene(State::Result);
		}
	}

	void draw() const override
	{
		BigCircle.drawFrame(5, Palette::White);
		SmallCircle.drawFrame(5, Palette::White);
		board.draw();
		if (isHinted)
			hintArrow.drawArrow(10, Vec2(15, 15), Palette::Pink);


		FontAsset(U"Menu")(U"うごかした回数 : {} 、　経過時間：{}"_fmt(board.moveNum, sw)).draw(100, 30, ColorF(0.25));

	}
};


//結果
class Result : public MyApp::Scene
{
private:
	Rect m_startButton = Rect(Arg::center = (PegSolitia::WindowSize / 2).movedBy(0, 0), 300, 60);
	Transition m_startTransition = Transition(0.4s, 0.2s);

	Rect m_exitButton = Rect(Arg::center = (PegSolitia::WindowSize / 2).movedBy(0, 100), 300, 60);
	Transition m_exitTransition = Transition(0.4s, 0.2s);

	Array<GameScore> scores;
public:

	Result(const InitData& init)
		: IScene(init)
	{
		//ハイスコアデータの読み込み
		{
			// バイナリファイルをオープン
			Deserializer<BinaryReader> reader(U"score.bin");

			if (!reader.getReader()) // もしオープンに失敗したら
			{
				scores =
					Array<GameScore>{
					GameScore{PegSolitia::Difficulty::Easy,10000s},
					GameScore{PegSolitia::Difficulty::Hard,10000s},
					GameScore{PegSolitia::Difficulty::Hell,10000s}
				};
			}
			else {
				// バイナリファイルからシリアライズ対応型のデータを読み込む
				// （自動でリサイズが行われる）
				reader(scores);
			}
			// reader のデストラクタで自動的にファイルがクローズされる
		}
		writeHighScore();
	}

	void writeHighScore()
	{
		//現在の記録と比較して高い方を残す。
		for (auto& data : scores) {
			if (this->getData().diff == data.diff) {
				if (this->getData().score < data.score) {
					data.score = this->getData().score;
				}
			}
		}

		// バイナリファイルをオープン
		Serializer<BinaryWriter> writer(U"score.bin");

		if (!writer.getWriter()) // もしオープンに失敗したら
		{
		}

		// シリアライズに対応したデータを記録
		writer(scores);

		// writer のデストラクタで自動的にファイルがクローズされる
	}

	void update() override
	{
		m_startTransition.update(m_startButton.mouseOver());
		m_exitTransition.update(m_exitButton.mouseOver());

		if (m_startButton.mouseOver() || m_exitButton.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		if (m_startButton.leftClicked())
		{
			changeScene(State::Title);
		}

		if (m_exitButton.leftClicked())
		{
			System::Exit();
		}
	}

	void draw() const override
	{
		String diffText;

		switch (this->getData().diff)
		{
		case PegSolitia::Difficulty::Easy:
			diffText = U"おめでとう";
			break;
		case PegSolitia::Difficulty::Hard:
			diffText = U"おめでとう！";
			break;
		case PegSolitia::Difficulty::Hell:
			diffText = U"称賛するよ！";
			break;
		default:
			break;
		}

		const Vec2 center((PegSolitia::WindowSize / 2).x, 120);
		FontAsset(U"Title")(diffText).drawAt(center.movedBy(4, 6), ColorF(0.0, 0.5));
		FontAsset(U"Title")(diffText).drawAt(center);
		FontAsset(U"Title")(Stopwatch(this->getData().score).format(U"M分ss秒")).drawAt(center + Vec2(0, 120));


		m_startButton.draw(ColorF(1.0, m_startTransition.value())).drawFrame(2);
		m_exitButton.draw(ColorF(1.0, m_exitTransition.value())).drawFrame(2);

		//これまでのハイスコアデータの表示
		s3d::Duration highScore;
		for (auto& data : scores) {
			if (this->getData().diff == data.diff) {
				highScore = data.score;
			}
		}

		FontAsset(U"Menu")(U"これまでのハイスコア：", Stopwatch(highScore).format(U"M分ss秒")).drawAt(m_exitButton.center() + Vec2(0, 200), ColorF(0.25));

		FontAsset(U"Menu")(U"タイトルにもどる").drawAt(m_startButton.center(), ColorF(0.25));
		FontAsset(U"Menu")(U"おわる").drawAt(m_exitButton.center(), ColorF(0.25));
	}
};

//メニュー
class Menu : public MyApp::Scene
{
private:
public:

	Menu(const InitData& init)
		: IScene(init)
	{
	}

	void update() override
	{

	}

	void draw() const override
	{

	}
};


void Main()
{
	// 使用するフォントアセットを登録
	FontAsset::Register(U"Title", 120, Typeface::Heavy);
	FontAsset::Register(U"Menu", 30, Typeface::Regular);
	FontAsset::Register(U"Score", 36, Typeface::Bold);

	Window::Resize(PegSolitia::WindowSize);

	// 背景色を設定
	Scene::SetBackground(ColorF(0.6, 0.7, 0.7));

	// シーンと遷移時の色を設定
	MyApp manager;
	manager
		.add<Title>(State::Title)
		.add<Tutorial>(State::Tutorial)
		.add<Game>(State::Game)
		.add<Result>(State::Result)
		.setFadeColor(ColorF(0.0));

	while (System::Update())
	{
		if (!manager.update())
		{
			break;
		}
	}
}