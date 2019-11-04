//////////////////////////////////////////////////////////////////////////////
// ExcelCreator Ver3.6

#ifndef __XLSXCRT_H__
#define __XLSXCRT_H__

//////////////////////////////////////////////////////////////////////////////
// 定数

// Excel バージョン
#define _ver2007				2007					// Excel 2007
#define _ver2010				2010					// Excel 2010
#define _ver2013				2013					// Excel 2013				// EC8-rev100
#define _ver2013S				-2013					// Excel 2013 StrictXML		// EC8-rev100
// カラーパレット
#define _xclAuto				0x80000000				// 自動
#define _xclBlack				0x80000008				// 黒
#define _xclWhite				0x80000009				// 白
#define _xclRed					0x8000000A				// 赤
#define _xclGreen				0x8000000B				// 緑
#define _xclBlue				0x8000000C				// 青
#define _xclYellow				0x8000000D				// 黄
#define _xclPink				0x8000000E				// ピンク
#define _xclCyan				0x8000000F				// 水
#define _xclDarkRed				0x80000010				// 濃い赤
#define _xclDarkGreen			0x80000011				// 濃い緑
#define _xclDrakBlue			0x80000012				// 濃い青
#define _xclDrakYellow			0x80000013				// 濃い黄
#define _xclPurple				0x80000014				// 紫
#define _xclBlueGreen			0x80000015				// 青緑
#define _xclGray25				0x80000016				// 25%灰色
#define _xclGray50				0x80000017				// 50%灰色
#define _xclGray				0x80000018				// グレー
#define _xclPlum				0x80000019				// プラム
#define _xclIvory				0x8000001A				// アイボリー
#define _xclLightBlue			0x8000001B				// 薄い水色
#define _xclDrakPurple			0x8000001C				// 濃い紫
#define _xclCoral				0x8000001D				// コーラル
#define _xclOceanBlue			0x8000001E				// オーシャンブルー
#define _xclIceBlue				0x8000001F				// アイスブルー
#define _xclDrakBlue2			0x80000020				// 濃い青
#define _xclPink2				0x80000021				// ピンク
#define _xclYellow2				0x80000022				// 黄色
#define _xclCyan2				0x80000023				// 水色
#define _xclPurple2				0x80000024				// 紫
#define _xclDarkRed2			0x80000025				// 濃い赤
#define _xclBlueGreen2			0x80000026				// 青緑
#define _xclBlue2				0x80000027				// 青
#define _xclSkyBlue				0x80000028				// スカイブルー
#define _xclLightCyan2			0x80000029				// 薄い水色
#define _xclLightGreen			0x8000002A				// 薄い緑
#define _xclLightYellow			0x8000002B				// 薄い黄色
#define _xclVeilBlue			0x8000002C				// ベールブルー
#define _xclRose				0x8000002D				// ローズ
#define _xclLavender			0x8000002E				// ラベンダー
#define _xclBeige				0x8000002F				// ベージュ
#define _xclLightBlue2			0x80000030				// 薄い青
#define _xclAquaBlue			0x80000031				// アクア
#define _xclLime				0x80000032				// ライム
#define _xclGold				0x80000033				// ゴールド
#define _xclLightOrange			0x80000034				// 薄いオレンジ
#define _xclOrange				0x80000035				// オレンジ
#define _xclBlueGrey			0x80000036				// ブルーグレー
#define _xclGray40				0x80000037				// 40%灰色
#define _xclDarkBlueGreen		0x80000038				// 濃い青緑
#define _xclSeaGreen			0x80000039				// シーグリーン
#define _xclDarkGreen2			0x8000003A				// 濃い緑
#define _xclOlive				0x8000003B				// オリーブ
#define _xclBrown				0x8000003C				// 茶
#define _xclPlum2				0x8000003D				// プラム
#define _xclIndigo				0x8000003E				// インディゴ
#define _xclGray80				0x8000003F				// 80%灰色
// フォントスタイル
#define _xfsNormal				0x00					// ノーマル
#define _xfsBold				0x01					// ボールド
#define _xfsItalic				0x02					// イタリック
#define _xfsStrike				0x04					// 取消し線
#define _xfsUp					0x08					// 上付き
#define _xfsDown				0x10					// 下付き
// テキストボックスフォントスタイル
#define _xtsNormal				0x00					// ノーマル
#define _xtsBold				0x01					// ボールド
#define _xtsItalic				0x02					// イタリック
// フォント下線
#define _xfuNone				0						// なし
#define _xfuNormal				1						// 下線
#define _xfuDouble				2						// 二重線
// 罫線の種類
#define _xbsNone				0x00000000				// なし
#define _xbsThin				0x01000000				// 実線
#define _xbsMedium				0x02000000				// 太線
#define _xbsDashed				0x03000000				// 破線
#define _xbsDotted				0x04000000				// 点線
#define _xbsThick				0x05000000				// 極太線
#define _xbsDouble				0x06000000				// 二重線
#define _xbsHair				0x07000000				// 細実線
#define _xbsMediumDashed		0x08000000				// 中破線
#define _xbsDashDot				0x09000000				// 細一点鎖線
#define _xbsMediumDashDot		0x0A000000				// 中一点鎖線
#define _xbsDashDotDot			0x0B000000				// 細二点鎖線
#define _xbsMediumDashDotDot	0x0C000000				// 中二点鎖線
#define _xbsSlantDashDot		0x0D000000				// 中一点斜鎖線
// パターンの種類
#define _xpnMask				0x1F000000				// マスク
#define _xpnSet(a)				((a)<<24)				// 設定／取得
#define _xpnGet(a)				(((a)&_xpnMask)>>24)
#define _xpnNone				0x01000000				// 01
#define _xpnSolid				0x02000000				// ｜
#define _xpnMediumGray			0x03000000				// ｜
#define _xpnDarkGray			0x04000000				// ｜
#define _xpnLightGray			0x05000000				// ｜
#define _xpnDarkHorizontal		0x06000000				// ｜
#define _xpnDarkVertical		0x07000000				// ｜
#define _xpnDarkDown			0x08000000				// ｜
#define _xpnDarkUp				0x09000000				// ｜
#define _xpnDarkGrid			0x0A000000				// 10
#define _xpnDarkTrellis			0x0B000000				// ｜
#define _xpnLightHorizontal		0x0C000000				// ｜
#define _xpnLightVertical		0x0D000000				// ｜
#define _xpnLightDown			0x0E000000				// ｜
#define _xpnLightUp				0x0F000000				// ｜
#define _xpnLightGrid			0x10000000				// ｜
#define _xpnLightTrellis		0x11000000				// ｜
#define _xpnGray125				0x12000000				// ｜
#define _xpnGray0625			0x13000000				// ｜
// ボックスの形状
#define _xbtBox					0						// 箱線
#define _xbtLtc					1						// 格子線
#define _xbtOver				2						// 上横線
#define _xbtUnder				3						// 下横線
#define _xbtLeft				4						// 左縦線
#define _xbtRight				5						// 右縦線
// 水平位置
#define _xphGeneral				20						// 標準
#define _xphLeft				21						// 左詰
#define _xphCenter				22						// 中央揃え
#define _xphRight				23						// 右詰
#define _xphFill				24						// 繰り返し
#define _xphJustify				25						// 両端揃え
#define _xphCenterContinuous	26						// 選択範囲内で中央
#define _xphDistributed			27						// 均等
// 垂直位置
#define _xpvTop					1						// 上詰め
#define _xpvCenter				2						// 中央揃え
#define _xpvBottom				3						// 下詰め
#define _xpvJustify				4						// 両端揃え
#define _xpvDistributed			5						// 均等
// 文字方向
#define _xptHorz				100						// 横
#define _xptVert				101						// 縦
#define _xptRvTurn90			102						// 反時計回りに90°
#define _xptTurn90				103						// 時計回りに90°
// 用紙サイズ
#define _xpsLetter				1						// レター
#define _xpsRejoer				3						// レジャー
#define _xpsRegal				5						// リーガル
#define _xpsExecutive			7						// エグゼクティブ
#define _xpsA3					8						// A3
#define _xpsA4					9						// A4
#define _xpsA5					11						// A5
#define _xpsB4					12						// B4
#define _xpsB5					13						// B5
// 用紙方向
#define _xorLandscape			0						// 横
#define _xorDefault				1						// 標準
#define _xorPortrait			2						// 縦
// データ、属性範囲
#define _xarMaxPoint			0						// 最大範囲
#define _xarEndPoint			1						// エンドポイント
// ページ中央位置
#define _xpcNone				0x00					// 指定なし
#define _xpcHorz				0x01					// 水平
#define _xpcVert				0x02					// 垂直
// ページの方向
#define _xpoRightFromLeft		0						// 左から右へ
#define _xpoBottomFromTop		1						// 上から下へ
// 変数差込モード
#define _xvmValidAll			0x00					// 全て差込
#define _xvmInvalidAnswer		0x01					// セルのみ
#define _xvmInvalidTextBox		0x02					// 計算式のみ
#define _xvmInvalidCellText		0x04					// テキストボックス
// ScaleMode
#define _xsmTwips				1						// 1/20 ポイント単位
#define _xsmPoints				2						// ポイント単位
#define _xsmPixels				3						// ピクセル単位
#define _xsmInches				5						// インチ単位
#define _xsmMillimeters			6						// ミリ単位
#define _xsmCentimeters			7						// センチ単位
// ヘッダ/フッター指定
#define _xhmNormal				0x00					// ノーマルモード：旧互換
#define _xhmOddEven				0x01					// 奇数偶数別モード
#define _xhmFirstPage			0x02					// 先頭ページ別モード
#define _xhmAll					0x03					// 奇数偶数先頭ページ別モード
// ヘッダ/フッター指定
#define _xhtNormal				0x00					// ノーマル・奇数ページ
#define _xhtEven				0x01					// 偶数ページ
#define _xhtFirst				0x02					// 先頭ページ
// エラー情報
#define _errDontOpen			1						// ファイルがオープンできない
#define _errFileHandle			2						// 無効なファイルハンドル
#define _errNotOpen				3						// 未オープンエラー
#define _errSheetNo				4						// シート番号エラー
#define _errWrite				5						// 書き込み時のエラー
#define _errAction				6						// 動作モードエラー
#define _errDataCat				7						// データ種別エラー
#define _errErrorValue			8						// エラー値
#define _errClear				20						// クリアエラー
#define _errCopy				21						// コピーエラー
#define _errPaste				22						// 貼り付けエラー
#define _errInsert				23						// 挿入エラー
#define _errDelete				24						// 削除エラー
#define _errLength				31						// 長さエラー
#define _errLocate				32						// 座標エラー
#define _errAttr				33						// 属性番号エラー
#define _errParam				34						// パラメータエラー
#define _errNoData				35						// データが無い
#define _errEndOfData			36						// データの終わり
#define _errVarPoint			37						// 指定した変数が存在しない
#define _errBreakCount			38						// 改ページ数が制限を越えました
#define _errMemory1				40						// メモリー不足エラー１
#define _errMemory2				41						// メモリー不足エラー２
#define _errOther				50						// 内部エラー
#define _errFunction			100						// 計算式形式エラー
#define _errCreate				300						// ファイル作成エラー
#define _errTempCreate			301						// 一時ファイルが作成できない
#define _errTempOpen			302						// 一時ファイルがオープンできない
#define _errStream				303						// ストリームエラー
#define _errProtect				304						// 保護されたシート
#define _errMutex				400						// ミューテックスエラー
// TextAnchor Type
#define _xacTop					1						// 上詰め
#define _xacCenter				2						// 中央揃え
#define _xacBottom				3						// 下詰め
// TextVertical Type
#define _xtvEaVert				1						// 縦書き
#define _xtvHorz				2						// 横書き
#define _xtvVert				4						// 右へ90°回転
#define _xtvVert270				5						// 左へ90°回転
#define _xtvWordArtVertRtl		7						// 縦書き(半角文字含む)
// TextAlign Type
#define _xalCenter				1						// 中央揃え
#define _xalLeft				5						// 左詰め
#define _xalRight				6						// 右詰め
// Shape Line Style
#define _xlsNone				0x00000000				// 線なし
#define _xlsNormal				0x00000001				// 実線
#define _xlsSysDot				0x00000002				// 丸点線
#define _xlsSysDash				0x00000003				// 角点線
#define _xlsDash				0x00000004				// 破線
#define _xlsDashDot				0x00000005				// 一点鎖線
#define _xlsLongDash			0x00000006				// 長破線
#define _xlsLongDashDot			0x00000007				// 長一点鎖線
#define _xlsLongDashDotDot		0x00000008				// 長二点鎖線
#define _xlsRound				0x00000009				// 細実線
#define _xltSingle				0x00000010				// 実線
#define _xltDouble				0x00000020				// 二重線
#define _xltThickThin			0x00000030				// 上が太い二重線
#define _xltThinThick			0x00000040				// 下が太い二重線
#define _xltTriple				0x00000050				// 三重線
// Arrow Type
#define _xatNone				0x00000000				// なし
#define _xatTriangle			0x00000001				// 三角形
#define _xatStealth				0x00000002				// ライン
#define _xatDiamond				0x00000003				// ひし形
#define _xatOval				0x00000004				// 丸
#define _xatArrow				0x00000005				// 鋭角
#define _xasSmall				0x00000000				// 小さく
#define _xasMedium				0x00000010				// 普通
#define _xasLarge				0x00000020				// 大きく
#define _xasShort				0x00000000				// 短い
#define _xasNormal				0x00000040				// 普通
#define _xasLong				0x00000080				// 長い
// Line Type
#define _xltLine				1						// 直線
#define _xltLineInv				2						// LineInv
#define _xltStraightConnector1	96						// 直線矢印コネクタ
#define _xltBentConnector2		97						// カギ線コネクタ2
#define _xltBentConnector3		98						// カギ線コネクタ3
#define _xltBentConnector4		99						// カギ線コネクタ4
#define _xltBentConnector5		100						// カギ線コネクタ5
#define _xltCurvedConnector2	101						// 曲線コネクタ2
#define _xltCurvedConnector3	102						// 曲線コネクタ3
#define _xltCurvedConnector4	103						// 曲線コネクタ4
#define _xltCurvedConnector5	104						// 曲線コネクタ5
// Shape Type
#define _xstTriangle					3				// 二等辺三角形
#define _xstRtTriangle					4				// 直角三角形
#define _xstRect						5				// 正方形/長方形
#define _xstDiamond						6				// ひし形
#define _xstParallelogram				7				// 平行四辺形
#define _xstTrapezoid					8				// 台形
#define _xstNonIsoscelesTrapezoid		9				// NonIsoscelesTrapezoid
#define _xstPentagon					10				// 五角形
#define _xstHexagon						11				// 六角形
#define _xstHeptagon					12				// 七角形
#define _xstOctagon						13				// 八角形
#define _xstDecagon						14				// 十角形
#define _xstDodecagon					15				// 十二角形
#define _xstStar4						16				// 星 4
#define _xstStar5						17				// 星 5
#define _xstStar6						18				// 星 6
#define _xstStar7						19				// 星 7
#define _xstStar8						20				// 星 8
#define _xstStar10						21				// 星 10
#define _xstStar12						22				// 星 12
#define _xstStar16						23				// 星 16
#define _xstStar24						24				// 星 24
#define _xstStar32						25				// 星 32
#define _xstRoundRect					26				// 角丸四角形
#define _xstRound1Rect					27				// 1 つの角を丸めた四角形
#define _xstRound2SameRect				28				// 片側の 2 つの角を丸めた四角形
#define _xstRound2DiagRect				29				// 対角する 2 つの角を丸めた四角形
#define _xstSnipRoundRect				30				// 1 つの角を丸めた四角形
#define _xstSnip1Rect					31				// 1 つの角を切り取った四角形
#define _xstSnip2SameRect				32				// 片側の 2 つの角を切り取った四角形
#define _xstSnip2DiagRect				33				// 対角する 2 つの角を切り取った四角形
#define _xstPlaque						34				// ブローチ
#define _xstEllipse						35				// 円/楕円
#define _xstTeardrop					36				// 涙形
#define _xstHomePlate					37				// ホームベース
#define _xstChevron						38				// 山形
#define _xstPieWedge					39				// PieWedge
#define _xstPie							40				// パイ
#define _xstBlockArc					41				// アーチ
#define _xstDonut						42				// ドーナツ
#define _xstNoSmoking					43				// 禁止
#define _xstRightArrow					44				// 右矢印
#define _xstLeftArrow					45				// 左矢印
#define _xstUpArrow						46				// 上矢印
#define _xstDownArrow					47				// 下矢印
#define _xstStripedRightArrow			48				// ストライプ矢印
#define _xstNotchedRightArrow			49				// V 字形矢印
#define _xstBentUpArrow					50				// 屈折矢印
#define _xstLeftRightArrow				51				// 左右矢印
#define _xstUpDownArrow					52				// 上下矢印
#define _xstLeftUpArrow					53				// 二方向矢印
#define _xstLeftRightUpArrow			54				// 三方向矢印
#define _xstQuadArrow					55				// 四方向矢印
#define _xstLeftArrowCallout			56				// 左矢印吹き出し
#define _xstRightArrowCallout			57				// 右矢印吹き出し
#define _xstUpArrowCallout				58				// 上矢印吹き出し
#define _xstDownArrowCallout			59				// 下矢印吹き出し
#define _xstLeftRightArrowCallout		60				// 左右矢印吹き出し
#define _xstUpDownArrowCallout			61				// 上下ブロック矢印付吹き出し
#define _xstQuadArrowCallout			62				// 四方向矢印吹き出し
#define _xstBentArrow					63				// 曲折矢印
#define _xstUturnArrow					64				// U ターン矢印
#define _xstCircularArrow				65				// 環状矢印
#define _xstLeftCircularArrow			66				// LeftCircularArrow
#define _xstLeftRightCircularArrow		67				// LeftRightCircularArrow
#define _xstCurvedRightArrow			68				// 右カーブ矢印
#define _xstCurvedLeftArrow				69				// 左カーブ矢印
#define _xstCurvedUpArrow				70				// 上カーブ矢印
#define _xstCurvedDownArrow				71				// 下カーブ矢印
#define _xstSwooshArrow					72				// SwooshArrow
#define _xstCube						73				// 直方体
#define _xstCan							74				// 円柱
#define _xstLightningBolt				75				// 稲妻
#define _xstHeart						76				// ハート
#define _xstSun							77				// 太陽
#define _xstMoon						78				// 月
#define _xstSmileyFace					79				// スマイル
#define _xstIrregularSeal1				80				// 爆発 1
#define _xstIrregularSeal2				81				// 爆発 2
#define _xstFoldedCorner				82				// メモ
#define _xstBevel						83				// 額縁
#define _xstFrame						84				// フレーム
#define _xstHalfFrame					85				// フレーム (半分)
#define _xstCorner						86				// L 字
#define _xstDiagStripe					87				// 斜め縞
#define _xstChord						88				// 弦
#define _xstArc							89				// 円弧
#define _xstLeftBracket					90				// 左大かっこ
#define _xstRightBracket				91				// 右大かっこ
#define _xstLeftBrace					92				// 左中かっこ
#define _xstRightBrace					93				// 右中かっこ
#define _xstBracketPair					94				// 大かっこ
#define _xstBracePair					95				// 中かっこ
#define _xstCallout1					105				// 線吹き出し 1 
#define _xstCallout2					106				// 線吹き出し 2 
#define _xstCallout3					107				// 線吹き出し 3 
#define _xstAccentCallout1				108				// 強調線吹き出し 1
#define _xstAccentCallout2				109				// 強調線吹き出し 2
#define _xstAccentCallout3				110				// 強調線吹き出し 3
#define _xstBorderCallout1				111				// 線吹き出し 1 (枠付き)
#define _xstBorderCallout2				112				// 線吹き出し 2 (枠付き)
#define _xstBorderCallout3				113				// 線吹き出し 3 (枠付き)
#define _xstAccentBorderCallout1		114				// 強調線吹き出し 1 (枠付き)
#define _xstAccentBorderCallout2		115				// 強調線吹き出し 2 (枠付き)
#define _xstAccentBorderCallout3		116				// 強調線吹き出し 3 (枠付き)
#define _xstWedgeRectCallout			117				// 四角形吹き出し
#define _xstWedgeRoundRectCallout		118				// 角丸四角形吹き出し
#define _xstWedgeEllipseCallout			119				// 円形吹き出し
#define _xstCloudCallout				120				// 雲形吹き出し
#define _xstCloud						121				// 雲
#define _xstRibbon						122				// 下リボン
#define _xstRibbon2						123				// 上リボン
#define _xstEllipseRibbon				124				// 下カーブ リボン
#define _xstEllipseRibbon2				125				// 上カーブ リボン
#define _xstLeftRightRibbon				126				// LeftRightRibbon
#define _xstVerticalScroll				127				// 縦巻き
#define _xstHorizontalScroll			128				// 横巻き
#define _xstWave						129				// 大波
#define _xstDoubleWave					130				// 小波
#define _xstPlus						131				// 十字形
#define _xstFlowChartProcess			132				// フローチャート : 処理
#define _xstFlowChartDecision			133				// フローチャート : 判断
#define _xstFlowChartInputOutput		134				// フローチャート : データ
#define _xstFlowChartPredefinedProcess	135				// フローチャート : 定義済み処理 
#define _xstFlowChartInternalStorage	136				// フローチャート : 内部記憶
#define _xstFlowChartDocument			137				// フローチャート : 書類
#define _xstFlowChartMultidocument		138				// フローチャート : 複数書類
#define _xstFlowChartTerminator			139				// フローチャート : 端子
#define _xstFlowChartPreparation		140				// フローチャート : 準備
#define _xstFlowChartManualInput		141				// フローチャート : 手操作入力
#define _xstFlowChartManualOperation	142				// フローチャート : 手作業
#define _xstFlowChartConnector			143				// フローチャート : 結合子
#define _xstFlowChartPunchedCard		144				// フローチャート : カード
#define _xstFlowChartPunchedTape		145				// フローチャート : せん孔テープ
#define _xstFlowChartSummingJunction	146				// フローチャート : 和接合
#define _xstFlowChartOr					147				// フローチャート : 論理和
#define _xstFlowChartCollate			148				// フローチャート : 照合
#define _xstFlowChartSort				149				// フローチャート : 分類
#define _xstFlowChartExtract			150				// フローチャート : 抜出し
#define _xstFlowChartMerge				151				// フローチャート : 組合せ
#define _xstFlowChartOfflineStorage		152				// フローチャート : OfflineStorage
#define _xstFlowChartOnlineStorage		153				// フローチャート : 記憶データ
#define _xstFlowChartMagneticTape		154				// フローチャート : 順次アクセス記憶
#define _xstFlowChartMagneticDisk		155				// フローチャート : 磁気ディスク
#define _xstFlowChartMagneticDrum		156				// フローチャート : 直接アクセス記憶
#define _xstFlowChartDisplay			157				// フローチャート : 表示
#define _xstFlowChartDelay				158				// フローチャート : 論理積ゲート
#define _xstFlowChartAlternateProcess	159				// フローチャート : 代替処理
#define _xstFlowChartOffpageConnector	160				// フローチャート : 他ページ結合子
#define _xstActionButtonBlank			161				// ActionButtonBlank
#define _xstActionButtonHome			162				// ActionButtonHome
#define _xstActionButtonHelp			163				// ActionButtonHelp
#define _xstActionButtonInformation		164				// ActionButtonInformation
#define _xstActionButtonForwardNext		165				// ActionButtonForwardNext
#define _xstActionButtonBackPrevious	166				// ActionButtonBackPrevious
#define _xstActionButtonEnd				167				// ActionButtonEnd
#define _xstActionButtonBeginning		168				// ActionButtonBeginning
#define _xstActionButtonReturn			169				// ActionButtonReturn
#define _xstActionButtonDocument		170				// ActionButtonDocument
#define _xstActionButtonSound			171				// ActionButtonSound
#define _xstActionButtonMovie			172				// ActionButtonMovie
#define _xstGear6						173				// Gear6
#define _xstGear9						174				// Gear9
#define _xstFunnel						175				// Funnel
#define _xstMathPlus					176				// 加算記号
#define _xstMathMinus					177				// 減算記号
#define _xstMathMultiply				178				// 乗算記号
#define _xstMathDivide					179				// 除算記号
#define _xstMathEqual					180				// 等号
#define _xstMathNotEqual				181				// 不等号
#define _xstCornerTabs					182				// CornerTabs
#define _xstSquareTabs					183				// SquareTabs
#define _xstPlaqueTabs					184				// PlaqueTabs
#define _xstChartX						185				// ChartX
#define _xstChartStar					186				// ChartStar
#define _xstChartPlus					187				// ChartPlus
// セルクリアモード
#define _clAll							0x00            // 全てクリア
#define _clValue						0x01            // 値のみ
#define _clAttr							0x02            // 属性のみ

//////////////////////////////////////////////////////////////////////////////
// C/C++
#ifndef   __CREATOROCX_RC_
#ifndef   __EXCELOCX_IDL_
#ifndef   __EXCELCRTOCX_H_

// Excel バージョン
#define ver2007					_ver2007				// Excel 2007
#define ver2010					_ver2010				// Excel 2010
#define ver2013					_ver2013				// Excel 2013			// EC8-rev100
#define ver2013S				_ver2013S				// Excel 2013 StrictXML	// EC8-rev100
// カラーパレット
#define xclAuto					_xclAuto				// 自動
#define xclBlack				_xclBlack				// 黒
#define xclWhite				_xclWhite				// 白
#define xclRed					_xclRed					// 赤
#define xclGreen				_xclGreen				// 緑
#define xclBlue					_xclBlue				// 青
#define xclYellow				_xclYellow				// 黄
#define xclPink					_xclPink				// ピンク
#define xclCyan					_xclCyan				// 水
#define xclDarkRed				_xclDarkRed				// 濃い赤
#define xclDarkGreen			_xclDarkGreen			// 濃い緑
#define xclDrakBlue				_xclDrakBlue			// 濃い青
#define xclDrakYellow			_xclDrakYellow			// 濃い黄
#define xclPurple				_xclPurple				// 紫
#define xclBlueGreen			_xclBlueGreen			// 青緑
#define xclGray25				_xclGray25				// 25%灰色
#define xclGray50				_xclGray50				// 50%灰色
#define xclGray					_xclGray				// グレー
#define xclPlum					_xclPlum				// プラム
#define xclIvory				_xclIvory				// アイボリー
#define xclLightBlue			_xclLightBlue			// 薄い水色
#define xclDrakPurple			_xclDrakPurple			// 濃い紫
#define xclCoral				_xclCoral				// コーラル
#define xclOceanBlue			_xclOceanBlue			// オーシャンブルー
#define xclIceBlue				_xclIceBlue				// アイスブルー
#define xclDrakBlue2			_xclDrakBlue2			// 濃い青
#define xclPink2				_xclPink2				// ピンク
#define xclYellow2				_xclYellow2				// 黄色
#define xclCyan2				_xclCyan2				// 水色
#define xclPurple2				_xclPurple2				// 紫
#define xclDarkRed2				_xclDarkRed2			// 濃い赤
#define xclBlueGreen2			_xclBlueGreen2			// 青緑
#define xclBlue2				_xclBlue2				// 青
#define xclSkyBlue				_xclSkyBlue				// スカイブルー
#define xclLightCyan2			_xclLightCyan2			// 薄い水色
#define xclLightGreen			_xclLightGreen			// 薄い緑
#define xclLightYellow			_xclLightYellow			// 薄い黄色
#define xclVeilBlue				_xclVeilBlue			// ベールブルー
#define xclRose					_xclRose				// ローズ
#define xclLavender				_xclLavender			// ラベンダー
#define xclBeige				_xclBeige				// ベージュ
#define xclLightBlue2			_xclLightBlue2			// 薄い青
#define xclAquaBlue				_xclAquaBlue			// アクア
#define xclLime					_xclLime				// ライム
#define xclGold					_xclGold				// ゴールド
#define xclLightOrange			_xclLightOrange			// 薄いオレンジ
#define xclOrange				_xclOrange				// オレンジ
#define xclBlueGrey				_xclBlueGrey			// ブルーグレー
#define xclGray40				_xclGray40				// 40%灰色
#define xclDarkBlueGreen		_xclDarkBlueGreen		// 濃い青緑
#define xclSeaGreen				_xclSeaGreen			// シーグリーン
#define xclDarkGreen2			_xclDarkGreen2			// 濃い緑
#define xclOlive				_xclOlive				// オリーブ
#define xclBrown				_xclBrown				// 茶
#define xclPlum2				_xclPlum2				// プラム
#define xclIndigo				_xclIndigo				// インディゴ
#define xclGray80				_xclGray80				// 80%灰色
// フォントスタイル
#define xfsNormal				_xfsNormal				// ノーマル
#define xfsBold					_xfsBold				// ボールド
#define xfsItalic				_xfsItalic				// イタリック
#define xfsStrike				_xfsStrike				// 取消し線
#define xfsUp					_xfsUp					// 上付き
#define xfsDown					_xfsDown				// 下付き
// テキストボックスフォントスタイル
#define xtsNormal				_xtsNormal				// ノーマル
#define xtsBold					_xtsBold				// ボールド
#define xtsItalic				_xtsItalic				// イタリック
// フォント下線
#define xfuNone					_xfuNone				// なし
#define xfuNormal				_xfuNormal				// 下線
#define xfuDouble				_xfuDouble				// 二重線
// 水平位置
#define xphGeneral				_xphGeneral				// 標準
#define xphLeft					_xphLeft				// 左詰
#define xphCenter				_xphCenter				// 中央揃え
#define xphRight				_xphRight				// 右詰
#define xphFill					_xphFill				// 繰り返し
#define xphJustify				_xphJustify				// 両端揃え
#define xphCenterContinuous		_xphCenterContinuous	// 選択範囲内で中央
#define xphDistributed			_xphDistributed			// 均等
// 垂直位置
#define xpvTop					_xpvTop					// 上詰め
#define xpvCenter				_xpvCenter				// 中央揃え
#define xpvBottom				_xpvBottom				// 下詰め
#define xpvJustify				_xpvJustify				// 両端揃え
#define xpvDistributed			_xpvDistributed			// 均等
// 文字方向
#define xptHorz					_xptHorz				// 横
#define xptVert					_xptVert				// 縦
#define xptRvTurn90				_xptRvTurn90			// 反時計回りに90°
#define xptTurn90				_xptTurn90				// 時計回りに90°
// 用紙サイズ
#define xpsLetter				_xpsLetter				// レター
#define xpsRejoer				_xpsRejoer				// レジャー
#define xpsRegal				_xpsRegal				// リーガル
#define xpsExecutive			_xpsExecutive			// エグゼクティブ
#define xpsA3					_xpsA3					// A3
#define xpsA4					_xpsA4					// A4
#define xpsA5					_xpsA5					// A5
#define xpsB4					_xpsB4					// B4
#define xpsB5					_xpsB5					// B5
// 用紙方向
#define xorLandscape			_xorLandscape			// 横
#define xorDefault				_xorDefault				// 標準
#define xorPortrait				_xorPortrait			// 縦
// データ、属性範囲
#define xarMaxPoint				_xarMaxPoint			// 最大範囲
#define xarEndPoint				_xarEndPoint			// エンドポイント
// ボックスの形状
#define xbtBox					_xbtBox					// 箱線
#define xbtLtc					_xbtLtc					// 格子線
#define xbtOver					_xbtOver				// 上横線
#define xbtUnder				_xbtUnder				// 下横線
#define xbtLeft					_xbtLeft				// 左縦線
#define xbtRight				_xbtRight				// 右縦線
// 罫線の線種
#define xbsNone					_xbsNone				// なし
#define xbsThin					_xbsThin				// 実線
#define xbsMedium				_xbsMedium				// 太線
#define xbsDashed				_xbsDashed				// 破線
#define xbsDotted				_xbsDotted				// 点線
#define xbsThick				_xbsThick				// 極太線
#define xbsDouble				_xbsDouble				// 二重線
#define xbsHair					_xbsHair				// 細実線
#define xbsMediumDashed			_xbsMediumDashed		// 中破線
#define xbsDashDot				_xbsDashDot				// 細一点鎖線
#define xbsMediumDashDot		_xbsMediumDashDot		// 中一点鎖線
#define xbsDashDotDot			_xbsDashDotDot			// 細二点鎖線
#define xbsMediumDashDotDot		_xbsMediumDashDotDot	// 中二点鎖線
#define xbsSlantDashDot			_xbsSlantDashDot		// 中一点斜鎖線
// パターン番号
#define xpnNone					_xpnNone				// 01
#define xpnSolid				_xpnSolid				// ｜
#define xpnMediumGray			_xpnMediumGray			// ｜
#define xpnDarkGray				_xpnDarkGray			// ｜
#define xpnLightGray			_xpnLightGray			// ｜
#define xpnDarkHorizontal		_xpnDarkHorizontal		// ｜
#define xpnDarkVertical			_xpnDarkVertical		// ｜
#define xpnDarkDown				_xpnDarkDown			// ｜
#define xpnDarkUp				_xpnDarkUp				// ｜
#define xpnDarkGrid				_xpnDarkGrid			// 10
#define xpnDarkTrellis			_xpnDarkTrellis			// ｜
#define xpnLightHorizontal		_xpnLightHorizontal		// ｜
#define xpnLightVertical		_xpnLightVertical		// ｜
#define xpnLightDown			_xpnLightDown			// ｜
#define xpnLightUp				_xpnLightUp				// ｜
#define xpnLightGrid			_xpnLightGrid			// ｜
#define xpnLightTrellis			_xpnLightTrellis		// ｜
#define xpnGray125				_xpnGray125				// ｜
#define xpnGray0625				_xpnGray0625			// ｜
// ページ中央位置
#define xpcNone					_xpcNone				// 指定なし
#define xpcHorz					_xpcHorz				// 水平
#define xpcVert					_xpcVert				// 垂直
// ページの方向
#define xpoRightFromLeft		_xpoRightFromLeft		// 左から右へ
#define xpoBottomFromTop		_xpoBottomFromTop		// 上から下へ
// 変数差込モード
#define xvmValidAll				_xvmValidAll			// 差込しない
#define xvmInvalidAnswer		_xvmInvalidAnswer		// セルのみ
#define xvmInvalidTextBox		_xvmInvalidTextBox		// 計算式のみ
#define xvmInvalidCellText		_xvmInvalidCellText		// テキストボックス
// ScaleMode
#define xsmTwips				_xsmTwips				// 1/20 ポイント単位
#define xsmPoints				_xsmPoints				// ポイント単位
#define xsmPixels				_xsmPixels				// ピクセル単位
#define xsmInches				_xsmInches				// インチ単位
#define xsmMillimeters			_xsmMillimeters			// ミリ単位
#define xsmCentimeters			_xsmCentimeters			// センチ単位
// ヘッダ/フッタモード
#define xhmNormal				_xhmNormal				// ノーマルモード：旧互換
#define xhmOddEven				_xhmOddEven				// 奇数偶数別モード
#define xhmFirstPage			_xhmFirstPage			// 先頭ページ別モード
#define xhmAll					_xhmAll					// 奇数偶数先頭ページ別モード
// ヘッダ/フッタタイプ
#define xhtNormal				_xhtNormal				// ノーマル・奇数ページ
#define	xhtEven					_xhtEven				// 偶数ページ
#define xhtFirst				_xhtFirst				// 先頭ページ
// Variant Type
#define vtStr					0						// 文字列
#define vtDouble				1						// 実数値
#define vtInt					2						// 整数
#define vtFunc					3						// 計算式
#define vtNull					4						// データなし
// エラー情報
#define errDontOpen				_errDontOpen			// ファイルがオープンできない
#define errFileHandle			_errFileHandle			// 無効なファイルハンドル
#define errNotOpen				_errNotOpen				// 未オープンエラー
#define errSheetNo				_errSheetNo				// シート番号エラー
#define errWrite				_errWrite				// 書き込み時のエラー
#define errAction				_errAction				// 動作モードエラー
#define errDataCat				_errDataCat				// データ種別エラー
#define errErrorValue			_errErrorValue			// エラー値
#define errClear				_errClear				// クリアエラー
#define errCopy					_errCopy				// コピーエラー
#define errPaste				_errPaste				// 貼り付けエラー
#define errInsert				_errInsert				// 挿入エラー
#define errDelete				_errDelete				// 削除エラー
#define errLength				_errLength				// 長さエラー
#define errLocate				_errLocate				// 座標エラー
#define errAttr					_errAttr				// 属性番号エラー
#define errParam				_errParam				// パラメータエラー
#define errNoData				_errNoData				// データが無い
#define errEndOfData			_errEndOfData			// データの終わり
#define errVarPoint				_errVarPoint			// 指定した変数が存在しない
#define errBreakCount			_errBreakCount			// 改ページ数が制限を越えました
#define errMemory1				_errMemory1				// メモリー不足エラー１
#define errMemory2				_errMemory2				// メモリー不足エラー２
#define errOther				_errOther				// 内部エラー
#define errFunction				_errFunction			// 計算式形式エラー
#define errCreate				_errCreate				// ファイル作成エラー
#define errTempCreate			_errTempCreate			// 一時ファイルが作成できない
#define errTempOpen				_errTempOpen			// 一時ファイルがオープンできない
#define errStream				_errStream				// ストリームエラー
#define errProtect				_errProtect				// 保護されたシート
#define errMutex				_errMutex				// ミューテックスエラー
// TextAnchor Type
#define xacTop					_xacTop					// 上詰め
#define xacCenter				_xacCenter				// 中央揃え
#define xacBottom				_xacBottom				// 下詰め
// TextVertical Type
#define xtvEaVert				_xtvEaVert				// 縦書き
#define xtvHorz					_xtvHorz				// 横書き
#define xtvVert					_xtvVert				// 右へ90°回転
#define xtvVert270				_xtvVert270				// 左へ90°回転
#define xtvWordArtVertRtl		_xtvWordArtVertRtl		// 縦書き(半角文字含む)
// TextAlign Type
#define xalCenter				_xalCenter				// 中央揃え
#define xalLeft					_xalLeft				// 左詰め
#define xalRight				_xalRight				// 右詰め
// Shape Line Style
#define xlsNone					_xlsNone				// 線なし
#define xlsNormal				_xlsNormal				// 実線
#define xlsSysDot				_xlsSysDot				// 丸点線
#define xlsSysDash				_xlsSysDash				// 角点線
#define xlsDash					_xlsDash				// 破線
#define xlsDashDot				_xlsDashDot				// 一点鎖線
#define xlsLongDash				_xlsLongDash			// 長破線
#define xlsLongDashDot			_xlsLongDashDot			// 長一点鎖線
#define xlsLongDashDotDot		_xlsLongDashDotDot		// 長二点鎖線
#define xlsRound				_xlsRound				// 細実線
#define xltSingle				_xltSingle				// 実線
#define xltDouble				_xltDouble				// 二重線
#define xltThickThin			_xltThickThin			// 上が太い二重線
#define xltThinThick			_xltThinThick			// 下が太い二重線
#define xltTriple				_xltTriple				// 三重線
// Arrow Type
#define xatNone					_xatNone				// なし
#define xatTriangle				_xatTriangle			// 三角形
#define xatStealth				_xatStealth				// ライン
#define xatDiamond				_xatDiamond				// ひし形
#define xatOval					_xatOval				// 丸
#define xatArrow				_xatArrow				// 鋭角
#define xasSmall				_xasSmall				// 小さく
#define xasMedium				_xasMedium				// 普通
#define xasLarge				_xasLarge				// 大きく
#define xasShort				_xasShort				// 短い
#define xasNormal				_xasNormal				// 普通
#define xasLong					_xasLong				// 長い
// Line Type
#define xltLine					_xltLine				// 直線
#define xltLineInv				_xltLineInv				// LineInv
#define xltStraightConnector1	_xltStraightConnector1	// 直線矢印コネクタ
#define xltBentConnector2		_xltBentConnector2		// カギ線コネクタ2
#define xltBentConnector3		_xltBentConnector3		// カギ線コネクタ3
#define xltBentConnector4		_xltBentConnector4		// カギ線コネクタ4
#define xltBentConnector5		_xltBentConnector5		// カギ線コネクタ5
#define xltCurvedConnector2		_xltCurvedConnector2	// 曲線コネクタ2
#define xltCurvedConnector3		_xltCurvedConnector3	// 曲線コネクタ3
#define xltCurvedConnector4		_xltCurvedConnector4	// 曲線コネクタ4
#define xltCurvedConnector5		_xltCurvedConnector5	// 曲線コネクタ5
// Shape Type
#define xstTriangle						_xstTriangle					// 二等辺三角形
#define xstRtTriangle					_xstRtTriangle					// 直角三角形
#define xstRect							_xstRect						// 正方形/長方形
#define xstDiamond						_xstDiamond						// ひし形
#define xstParallelogram				_xstParallelogram				// 平行四辺形
#define xstTrapezoid					_xstTrapezoid					// 台形
#define xstNonIsoscelesTrapezoid		_xstNonIsoscelesTrapezoid		// NonIsoscelesTrapezoid
#define xstPentagon						_xstPentagon					// 五角形
#define xstHexagon						_xstHexagon						// 六角形
#define xstHeptagon						_xstHeptagon					// 七角形
#define xstOctagon						_xstOctagon						// 八角形
#define xstDecagon						_xstDecagon						// 十角形
#define xstDodecagon					_xstDodecagon					// 十二角形
#define xstStar4						_xstStar4						// 星 4
#define xstStar5						_xstStar5						// 星 5
#define xstStar6						_xstStar6						// 星 6
#define xstStar7						_xstStar7						// 星 7
#define xstStar8						_xstStar8						// 星 8
#define xstStar10						_xstStar10						// 星 10
#define xstStar12						_xstStar12						// 星 12
#define xstStar16						_xstStar16						// 星 16
#define xstStar24						_xstStar24						// 星 24
#define xstStar32						_xstStar32						// 星 32
#define xstRoundRect					_xstRoundRect					// 角丸四角形
#define xstRound1Rect					_xstRound1Rect					// 1 つの角を丸めた四角形
#define xstRound2SameRect				_xstRound2SameRect				// 片側の 2 つの角を丸めた四角形
#define xstRound2DiagRect				_xstRound2DiagRect				// 対角する 2 つの角を丸めた四角形
#define xstSnipRoundRect				_xstSnipRoundRect				// 1 つの角を丸めた四角形
#define xstSnip1Rect					_xstSnip1Rect					// 1 つの角を切り取った四角形
#define xstSnip2SameRect				_xstSnip2SameRect				// 片側の 2 つの角を切り取った四角形
#define xstSnip2DiagRect				_xstSnip2DiagRect				// 対角する 2 つの角を切り取った四角形
#define xstPlaque						_xstPlaque						// ブローチ
#define xstEllipse						_xstEllipse						// 円/楕円
#define xstTeardrop						_xstTeardrop					// 涙形
#define xstHomePlate					_xstHomePlate					// ホームベース
#define xstChevron						_xstChevron						// 山形
#define xstPieWedge						_xstPieWedge					// PieWedge
#define xstPie							_xstPie							// パイ
#define xstBlockArc						_xstBlockArc					// アーチ
#define xstDonut						_xstDonut						// ドーナツ
#define xstNoSmoking					_xstNoSmoking					// 禁止
#define xstRightArrow					_xstRightArrow					// 右矢印
#define xstLeftArrow					_xstLeftArrow					// 左矢印
#define xstUpArrow						_xstUpArrow						// 上矢印
#define xstDownArrow					_xstDownArrow					// 下矢印
#define xstStripedRightArrow			_xstStripedRightArrow			// ストライプ矢印
#define xstNotchedRightArrow			_xstNotchedRightArrow			// V 字形矢印
#define xstBentUpArrow					_xstBentUpArrow					// 屈折矢印
#define xstLeftRightArrow				_xstLeftRightArrow				// 左右矢印
#define xstUpDownArrow					_xstUpDownArrow					// 上下矢印
#define xstLeftUpArrow					_xstLeftUpArrow					// 二方向矢印
#define xstLeftRightUpArrow				_xstLeftRightUpArrow			// 三方向矢印
#define xstQuadArrow					_xstQuadArrow					// 四方向矢印
#define xstLeftArrowCallout				_xstLeftArrowCallout			// 左矢印吹き出し
#define xstRightArrowCallout			_xstRightArrowCallout			// 右矢印吹き出し
#define xstUpArrowCallout				_xstUpArrowCallout				// 上矢印吹き出し
#define xstDownArrowCallout				_xstDownArrowCallout			// 下矢印吹き出し
#define xstLeftRightArrowCallout		_xstLeftRightArrowCallout		// 左右矢印吹き出し
#define xstUpDownArrowCallout			_xstUpDownArrowCallout			// 上下ブロック矢印付吹き出し
#define xstQuadArrowCallout				_xstQuadArrowCallout			// 四方向矢印吹き出し
#define xstBentArrow					_xstBentArrow					// 曲折矢印
#define xstUturnArrow					_xstUturnArrow					// U ターン矢印
#define xstCircularArrow				_xstCircularArrow				// 環状矢印
#define xstLeftCircularArrow			_xstLeftCircularArrow			// LeftCircularArrow
#define xstLeftRightCircularArrow		_xstLeftRightCircularArrow		// LeftRightCircularArrow
#define xstCurvedRightArrow				_xstCurvedRightArrow			// 右カーブ矢印
#define xstCurvedLeftArrow				_xstCurvedLeftArrow				// 左カーブ矢印
#define xstCurvedUpArrow				_xstCurvedUpArrow				// 上カーブ矢印
#define xstCurvedDownArrow				_xstCurvedDownArrow				// 下カーブ矢印
#define xstSwooshArrow					_xstSwooshArrow					// SwooshArrow
#define xstCube							_xstCube						// 直方体
#define xstCan							_xstCan							// 円柱
#define xstLightningBolt				_xstLightningBolt				// 稲妻
#define xstHeart						_xstHeart						// ハート
#define xstSun							_xstSun							// 太陽
#define xstMoon							_xstMoon						// 月
#define xstSmileyFace					_xstSmileyFace					// スマイル
#define xstIrregularSeal1				_xstIrregularSeal1				// 爆発 1
#define xstIrregularSeal2				_xstIrregularSeal2				// 爆発 2
#define xstFoldedCorner					_xstFoldedCorner				// メモ
#define xstBevel						_xstBevel						// 額縁
#define xstFrame						_xstFrame						// フレーム
#define xstHalfFrame					_xstHalfFrame					// フレーム (半分)
#define xstCorner						_xstCorner						// L 字
#define xstDiagStripe					_xstDiagStripe					// 斜め縞
#define xstChord						_xstChord						// 弦
#define xstArc							_xstArc							// 円弧
#define xstLeftBracket					_xstLeftBracket					// 左大かっこ
#define xstRightBracket					_xstRightBracket				// 右大かっこ
#define xstLeftBrace					_xstLeftBrace					// 左中かっこ
#define xstRightBrace					_xstRightBrace					// 右中かっこ
#define xstBracketPair					_xstBracketPair					// 大かっこ
#define xstBracePair					_xstBracePair					// 中かっこ
#define xstCallout1						_xstCallout1					// 線吹き出し 1 
#define xstCallout2						_xstCallout2					// 線吹き出し 2 
#define xstCallout3						_xstCallout3					// 線吹き出し 3 
#define xstAccentCallout1				_xstAccentCallout1				// 強調線吹き出し 1
#define xstAccentCallout2				_xstAccentCallout2				// 強調線吹き出し 2
#define xstAccentCallout3				_xstAccentCallout3				// 強調線吹き出し 3
#define xstBorderCallout1				_xstBorderCallout1				// 線吹き出し 1 (枠付き)
#define xstBorderCallout2				_xstBorderCallout2				// 線吹き出し 2 (枠付き)
#define xstBorderCallout3				_xstBorderCallout3				// 線吹き出し 3 (枠付き)
#define xstAccentBorderCallout1			_xstAccentBorderCallout1		// 強調線吹き出し 1 (枠付き)
#define xstAccentBorderCallout2			_xstAccentBorderCallout2		// 強調線吹き出し 2 (枠付き)
#define xstAccentBorderCallout3			_xstAccentBorderCallout3		// 強調線吹き出し 3 (枠付き)
#define xstWedgeRectCallout				_xstWedgeRectCallout			// 四角形吹き出し
#define xstWedgeRoundRectCallout		_xstWedgeRoundRectCallout		// 角丸四角形吹き出し
#define xstWedgeEllipseCallout			_xstWedgeEllipseCallout			// 円形吹き出し
#define xstCloudCallout					_xstCloudCallout				// 雲形吹き出し
#define xstCloud						_xstCloud						// 雲
#define xstRibbon						_xstRibbon						// 下リボン
#define xstRibbon2						_xstRibbon2						// 上リボン
#define xstEllipseRibbon				_xstEllipseRibbon				// 下カーブ リボン
#define xstEllipseRibbon2				_xstEllipseRibbon2				// 上カーブ リボン
#define xstLeftRightRibbon				_xstLeftRightRibbon				// LeftRightRibbon
#define xstVerticalScroll				_xstVerticalScroll				// 縦巻き
#define xstHorizontalScroll				_xstHorizontalScroll			// 横巻き
#define xstWave							_xstWave						// 大波
#define xstDoubleWave					_xstDoubleWave					// 小波
#define xstPlus							_xstPlus						// 十字形
#define xstFlowChartProcess				_xstFlowChartProcess			// フローチャート : 処理
#define xstFlowChartDecision			_xstFlowChartDecision			// フローチャート : 判断
#define xstFlowChartInputOutput			_xstFlowChartInputOutput		// フローチャート : データ
#define xstFlowChartPredefinedProcess	_xstFlowChartPredefinedProcess	// フローチャート : 定義済み処理 
#define xstFlowChartInternalStorage		_xstFlowChartInternalStorage	// フローチャート : 内部記憶
#define xstFlowChartDocument			_xstFlowChartDocument			// フローチャート : 書類
#define xstFlowChartMultidocument		_xstFlowChartMultidocument		// フローチャート : 複数書類
#define xstFlowChartTerminator			_xstFlowChartTerminator			// フローチャート : 端子
#define xstFlowChartPreparation			_xstFlowChartPreparation		// フローチャート : 準備
#define xstFlowChartManualInput			_xstFlowChartManualInput		// フローチャート : 手操作入力
#define xstFlowChartManualOperation		_xstFlowChartManualOperation	// フローチャート : 手作業
#define xstFlowChartConnector			_xstFlowChartConnector			// フローチャート : 結合子
#define xstFlowChartPunchedCard			_xstFlowChartPunchedCard		// フローチャート : カード
#define xstFlowChartPunchedTape			_xstFlowChartPunchedTape		// フローチャート : せん孔テープ
#define xstFlowChartSummingJunction		_xstFlowChartSummingJunction	// フローチャート : 和接合
#define xstFlowChartOr					_xstFlowChartOr					// フローチャート : 論理和
#define xstFlowChartCollate				_xstFlowChartCollate			// フローチャート : 照合
#define xstFlowChartSort				_xstFlowChartSort				// フローチャート : 分類
#define xstFlowChartExtract				_xstFlowChartExtract			// フローチャート : 抜出し
#define xstFlowChartMerge				_xstFlowChartMerge				// フローチャート : 組合せ
#define xstFlowChartOfflineStorage		_xstFlowChartOfflineStorage		// フローチャート : OfflineStorage
#define xstFlowChartOnlineStorage		_xstFlowChartOnlineStorage		// フローチャート : 記憶データ
#define xstFlowChartMagneticTape		_xstFlowChartMagneticTape		// フローチャート : 順次アクセス記憶
#define xstFlowChartMagneticDisk		_xstFlowChartMagneticDisk		// フローチャート : 磁気ディスク
#define xstFlowChartMagneticDrum		_xstFlowChartMagneticDrum		// フローチャート : 直接アクセス記憶
#define xstFlowChartDisplay				_xstFlowChartDisplay			// フローチャート : 表示
#define xstFlowChartDelay				_xstFlowChartDelay				// フローチャート : 論理積ゲート
#define xstFlowChartAlternateProcess	_xstFlowChartAlternateProcess	// フローチャート : 代替処理
#define xstFlowChartOffpageConnector	_xstFlowChartOffpageConnector	// フローチャート : 他ページ結合子
#define xstActionButtonBlank			_xstActionButtonBlank			// ActionButtonBlank
#define xstActionButtonHome				_xstActionButtonHome			// ActionButtonHome
#define xstActionButtonHelp				_xstActionButtonHelp			// ActionButtonHelp
#define xstActionButtonInformation		_xstActionButtonInformation		// ActionButtonInformation
#define xstActionButtonForwardNext		_xstActionButtonForwardNext		// ActionButtonForwardNext
#define xstActionButtonBackPrevious		_xstActionButtonBackPrevious	// ActionButtonBackPrevious
#define xstActionButtonEnd				_xstActionButtonEnd				// ActionButtonEnd
#define xstActionButtonBeginning		_xstActionButtonBeginning		// ActionButtonBeginning
#define xstActionButtonReturn			_xstActionButtonReturn			// ActionButtonReturn
#define xstActionButtonDocument			_xstActionButtonDocument		// ActionButtonDocument
#define xstActionButtonSound			_xstActionButtonSound			// ActionButtonSound
#define xstActionButtonMovie			_xstActionButtonMovie			// ActionButtonMovie
#define xstGear6						_xstGear6						// Gear6
#define xstGear9						_xstGear9						// Gear9
#define xstFunnel						_xstFunnel						// Funnel
#define xstMathPlus						_xstMathPlus					// 加算記号
#define xstMathMinus					_xstMathMinus					// 減算記号
#define xstMathMultiply					_xstMathMultiply				// 乗算記号
#define xstMathDivide					_xstMathDivide					// 除算記号
#define xstMathEqual					_xstMathEqual					// 等号
#define xstMathNotEqual					_xstMathNotEqual				// 不等号
#define xstCornerTabs					_xstCornerTabs					// CornerTabs
#define xstSquareTabs					_xstSquareTabs					// SquareTabs
#define xstPlaqueTabs					_xstPlaqueTabs					// PlaqueTabs
#define xstChartX						_xstChartX						// ChartX
#define xstChartStar					_xstChartStar					// ChartStar
#define xstChartPlus					_xstChartPlus					// ChartPlus
// セルクリアモード
#define clAll							_clAll							// 全てクリア
#define clValue							_clValue						// 値のみ
#define clAttr							_clAttr							// 属性のみ

//////////////////////////////////////////////////////////////////////////////
// ExcelCreator API

#ifdef __cplusplus
extern "C" {  // Assume C declarations for C++
#endif // __cplusplus

    // イベント関数
    typedef void (CALLBACK* XLSX_ONERRORPROC)(long *pError, LPARAM lp);
    // インスタンス生成
    BOOL PASCAL XlsxCreate(LPDWORD pID);
    BOOL PASCAL XlsxRelease(DWORD dwID);
    // Xlsx.
    BOOL PASCAL XlsxAbout(DWORD dwID);
    BOOL PASCAL XlsxCreateBook(DWORD dwID, LPCSTR pszFileName, int nVersion, int nSheetMax, int* pBookID);
    BOOL PASCAL XlsxCreateBookW(DWORD dwID, LPCWSTR pszFileName, int nVersion, int nSheetMax, int* pBookID);
    BOOL PASCAL XlsxOpenBook(DWORD dwID, LPCSTR pszFileName, LPCSTR pszOverlay, int* pBookID);
    BOOL PASCAL XlsxOpenBookW(DWORD dwID, LPCWSTR pszFileName, LPCWSTR pszOverlay, int* pBookID);
    BOOL PASCAL XlsxReadBook(DWORD dwID, LPCSTR pszFileName, int* pBookID);
    BOOL PASCAL XlsxReadBookW(DWORD dwID, LPCWSTR pszFileName, int* pBookID);
    BOOL PASCAL XlsxCloseBook(DWORD dwID, BOOL bSave);
    BOOL PASCAL XlsxSelectBook(DWORD dwID, int newVal);
    BOOL PASCAL XlsxPutDefFontName(DWORD dwID, LPCSTR newVal);
    BOOL PASCAL XlsxPutDefFontNameW(DWORD dwID, LPCWSTR newVal);
    BOOL PASCAL XlsxPutDefFontPoint(DWORD dwID, double newVal);
    BOOL PASCAL XlsxPutSheetNo(DWORD dwID, int newVal);
    BOOL PASCAL XlsxPutSheetName(DWORD dwID, LPCSTR newVal);
    BOOL PASCAL XlsxPutSheetNameW(DWORD dwID, LPCWSTR newVal);
    BOOL PASCAL XlsxGetSheetName(DWORD dwID, LPSTR pVal);
    BOOL PASCAL XlsxGetSheetNameW(DWORD dwID, LPWSTR pVal);
    int PASCAL XlsxGetSheetCount(DWORD dwID);
    BOOL PASCAL XlsxGetSheetName2(DWORD dwID, int nSheetNo, LPSTR pVal);
    BOOL PASCAL XlsxGetSheetName2W(DWORD dwID, int nSheetNo, LPWSTR pVal);
    int PASCAL XlsxGetSheetNo2(DWORD dwID, LPCSTR pszSheetName);
    int PASCAL XlsxGetSheetNo2W(DWORD dwID, LPCWSTR pszSheetName);
    BOOL PASCAL XlsxPutActiveSheet(DWORD dwID, int newVal);
    int PASCAL XlsxGetActiveSheet(DWORD dwID);
    BOOL PASCAL XlsxAddSheet(DWORD dwID, int nCount, int nInsertPos);
    BOOL PASCAL XlsxDelSheet(DWORD dwID, int nSheetNo, int nCount);
    BOOL PASCAL XlsxCopySheet(DWORD dwID, int nSource, int nInsertPos, LPCSTR pszSheetName);
    BOOL PASCAL XlsxCopySheetW(DWORD dwID, int nSource, int nInsertPos, LPCWSTR pszSheetName);
    BOOL PASCAL XlsxHeader(DWORD dwID, LPCSTR pszLeft, LPCSTR pszCenter, LPCSTR pszRight);
    BOOL PASCAL XlsxFooter(DWORD dwID, LPCSTR pszLeft, LPCSTR pszCenter, LPCSTR pszRight);
    BOOL PASCAL XlsxHeader2(DWORD dwID, LPCSTR pszLeft, LPCSTR pszCenter, LPCSTR pszRight, int nMode);
    BOOL PASCAL XlsxFooter2(DWORD dwID, LPCSTR pszLeft, LPCSTR pszCenter, LPCSTR pszRight, int nMode);
    BOOL PASCAL XlsxHeaderW(DWORD dwID, LPCWSTR pszLeft, LPCWSTR pszCenter, LPCWSTR pszRight);
    BOOL PASCAL XlsxFooterW(DWORD dwID, LPCWSTR pszLeft, LPCWSTR pszCenter, LPCWSTR pszRight);
    BOOL PASCAL XlsxHeader2W(DWORD dwID, LPCWSTR pszLeft, LPCWSTR pszCenter, LPCWSTR pszRight, int nMode);
    BOOL PASCAL XlsxFooter2W(DWORD dwID, LPCWSTR pszLeft, LPCWSTR pszCenter, LPCWSTR pszRight, int nMode);
    BOOL PASCAL XlsxPutHeaderFooterMode(DWORD dwID, int newVal);
    BOOL PASCAL XlsxPageMode(DWORD dwID, int nPaperSize, int nScale, int nOrientation, int nHeaderMargin, int nFooterMargin);
    BOOL PASCAL XlsxSetMargin(DWORD dwID, int nLeft, int nTop, int nRight, int nBottom);
    BOOL PASCAL XlsxSizeFree(DWORD dwID, double dblWidth, double dblHeight, LPCSTR pszPaperName, LPCSTR pszPrinterName);
    BOOL PASCAL XlsxSizeFreeW(DWORD dwID, double dblWidth, double dblHeight, LPCWSTR pszPaperName, LPCWSTR pszPrinterName);
    BOOL PASCAL XlsxPrintArea(DWORD dwID, int sx, int sy, int ex, int ey);
    BOOL PASCAL XlsxPrintTitle(DWORD dwID, int sy, int ey, int sx, int ex);
    BOOL PASCAL XlsxPutCenter(DWORD dwID, int newVal);
    BOOL PASCAL XlsxPutPageOrder(DWORD dwID, int newVal);
	BOOL PASCAL XlsxPutModeGrid(DWORD dwID, BOOL newVal);
    BOOL PASCAL XlsxPutZoom(DWORD dwID, int newVal);
	BOOL PASCAL XlsxPutKeyWord(DWORD dwID, LPCSTR newVal);
	BOOL PASCAL XlsxPutKeyWordW(DWORD dwID, LPCWSTR newVal);
	BOOL PASCAL XlsxPutVarInsertMode(DWORD dwID, int newVal);
	BOOL PASCAL XlsxPutScaleMode(DWORD dwID, int newVal);
	int PASCAL XlsxGetScaleMode(DWORD dwID);
    BOOL PASCAL XlsxRowDelete(DWORD dwID, int sy, int nCount);
    BOOL PASCAL XlsxRowClear(DWORD dwID, int sy, int nCount);
    BOOL PASCAL XlsxRowInsert(DWORD dwID, int sy, int nCount);
    BOOL PASCAL XlsxRowCopy(DWORD dwID, int sy, int dy);
    BOOL PASCAL XlsxRowPaste(DWORD dwID, int dy);
    BOOL PASCAL XlsxColumnDelete(DWORD dwID, int sx, int nCount);
    BOOL PASCAL XlsxColumnClear(DWORD dwID, int sx, int nCount);
    BOOL PASCAL XlsxColumnInsert(DWORD dwID, int sx, int nCount);
    BOOL PASCAL XlsxColumnCopy(DWORD dwID, int sx, int dx);
    BOOL PASCAL XlsxColumnPaste(DWORD dwID, int dx);
    BOOL PASCAL XlsxGetMaxData(DWORD dwID, int* ex, int* ey, int pt);
    BOOL PASCAL XlsxGetMaxArea(DWORD dwID, int* ex, int* ey, int pt);
	BOOL PASCAL XlsxVarCheck(DWORD dwID, LPCSTR pszVarName);
	BOOL PASCAL XlsxVarCheckW(DWORD dwID, LPCWSTR pszVarName);
	BOOL PASCAL XlsxGetVarNamePos(DWORD dwID, LPCSTR pszVarName, int* cx, int* cy, int nIndex);
	BOOL PASCAL XlsxGetVarNamePosW(DWORD dwID, LPCWSTR pszVarName, int* cx, int* cy, int nIndex);
	BOOL PASCAL XlsxGetNamePos(DWORD dwID, LPCSTR pszName, int* cx, int* cy);
	BOOL PASCAL XlsxGetNamePosW(DWORD dwID, LPCWSTR pszName, int* cx, int* cy);
	BOOL PASCAL XlsxInitFormulaAnswer(DWORD dwID, BOOL bInit);
	BOOL PASCAL XlsxImageFileNonDeleteMode(DWORD dwID, BOOL bInit);
	BOOL PASCAL XlsxFullCalcOnLoad(DWORD dwID, BOOL bLoad);
	BOOL PASCAL XlsxChangeRefSheetAddress(DWORD dwID, BOOL bLoad);
    int PASCAL XlsxGetErrorNo(DWORD dwID);
    BOOL PASCAL XlsxGetErrorMessage(DWORD dwID, LPSTR pVal);
    BOOL PASCAL XlsxGetErrorMessageW(DWORD dwID, LPWSTR pVal);
    // Xlsx.Cell/Pos
    BOOL PASCAL XlsxCell(DWORD dwID, LPCSTR pszCell, int xoff, int yoff);
    BOOL PASCAL XlsxCellW(DWORD dwID, LPCWSTR pszCell, int xoff, int yoff);
    BOOL PASCAL XlsxPos(DWORD dwID, int sx, int sy, int ex, int ey);
    // Xlsx.Cell/Pos.Value
    BOOL PASCAL XlsxPutValue(DWORD dwID, VARIANT newVal);
    BOOL PASCAL XlsxGetValue(DWORD dwID, VARIANT* pVal);
    BOOL PASCAL XlsxPutValue2(DWORD dwID, VARIANT newVal, int xfno);
    BOOL PASCAL XlsxPutLong(DWORD dwID, long newVal);
    long PASCAL XlsxGetLong(DWORD dwID);
    BOOL PASCAL XlsxPutLong2(DWORD dwID, long newVal, int xfno);
    BOOL PASCAL XlsxPutDouble(DWORD dwID, double newVal);
    double PASCAL XlsxGetDouble(DWORD dwID);
    BOOL PASCAL XlsxPutDouble2(DWORD dwID, double newVal, int xfno);
    BOOL PASCAL XlsxPutString(DWORD dwID, LPCSTR newVal);
    BOOL PASCAL XlsxGetString(DWORD dwID, LPSTR pVal);
    BOOL PASCAL XlsxPutString2(DWORD dwID, LPCSTR newVal, int xfno);
    BOOL PASCAL XlsxPutStringW(DWORD dwID, LPCWSTR newVal);
    BOOL PASCAL XlsxGetStringW(DWORD dwID, LPWSTR pVal);
    BOOL PASCAL XlsxPutString2W(DWORD dwID, LPCWSTR newVal, int xfno);
    BOOL PASCAL XlsxPutInt64(DWORD dwID, INT64 newVal);
    INT64 PASCAL XlsxGetInt64(DWORD dwID);
    BOOL PASCAL XlsxPutInt642(DWORD dwID, INT64 newVal, int xfno);
    BOOL PASCAL XlsxPutFunc(DWORD dwID, LPCSTR pszFunc, VARIANT vAnswer);
    BOOL PASCAL XlsxPutFuncW(DWORD dwID, LPCWSTR pszFunc, VARIANT vAnswer);
    BOOL PASCAL XlsxPutColWidth(DWORD dwID, double newVal);
    double PASCAL XlsxGetColWidth(DWORD dwID);
    BOOL PASCAL XlsxPutRowHeight(DWORD dwID, double newVal);
    double PASCAL XlsxGetRowHeight(DWORD dwID);
    BOOL PASCAL XlsxPutBreak(DWORD dwID, BOOL newVal);
    int PASCAL XlsxGetAttrNo(DWORD dwID);
    BOOL PASCAL XlsxCellClear(DWORD dwID);
    BOOL PASCAL XlsxCellCopy(DWORD dwID, LPCSTR pszCell);
    BOOL PASCAL XlsxCellCopyW(DWORD dwID, LPCWSTR pszCell);
    BOOL PASCAL XlsxCellPaste(DWORD dwID);
    BOOL PASCAL XlsxCellClear2(DWORD dwID, int nClearMode);
    // Xlsx.Cell/Pos.Attr
    BOOL PASCAL XlsxAttrPutFormat(DWORD dwID, LPCSTR newVal);
    BOOL PASCAL XlsxAttrPutFormatW(DWORD dwID, LPCWSTR newVal);
    BOOL PASCAL XlsxAttrPutFontName(DWORD dwID, LPCSTR newVal);
    BOOL PASCAL XlsxAttrPutFontNameW(DWORD dwID, LPCWSTR newVal);
    BOOL PASCAL XlsxAttrPutFontPoint(DWORD dwID, double newVal);
    BOOL PASCAL XlsxAttrPutFontColor(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutFontStyle(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutFontULine(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutLineTop(DWORD dwID, int nBorderStyle, int nColor);
    BOOL PASCAL XlsxAttrPutLineBottom(DWORD dwID, int nBorderStyle, int nColor);
    BOOL PASCAL XlsxAttrPutLineLeft(DWORD dwID, int nBorderStyle, int nColor);
    BOOL PASCAL XlsxAttrPutLineRight(DWORD dwID, int nBorderStyle, int nColor);
    BOOL PASCAL XlsxAttrPutLineLeftUp(DWORD dwID, int nBorderStyle, int nColor);
    BOOL PASCAL XlsxAttrPutLineRightUp(DWORD dwID, int nBorderStyle, int nColor);
    BOOL PASCAL XlsxAttrPutPattern(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutForeColor(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutBackColor(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutPosHorz(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutPosVert(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutPosTurn(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutOverReturn(DWORD dwID, BOOL newVal);
    BOOL PASCAL XlsxAttrPutFit(DWORD dwID, BOOL newVal);
    BOOL PASCAL XlsxAttrPutJoint(DWORD dwID, BOOL newVal);
    BOOL PASCAL XlsxAttrPutBox(DWORD dwID, int nBorderStyle, int nBoxType, int nColor);
    // Xlsx.Cell/Pos.Drawing
	BOOL PASCAL XlsxDrawingInit(DWORD dwID);
    BOOL PASCAL XlsxDrawingPutLineColor(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutLineWeight(DWORD dwID, double newVal);
    BOOL PASCAL XlsxDrawingPutLineStyle(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutArrowTypeS(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutArrowTypeE(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutFillColor(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutFontName(DWORD dwID, LPCSTR newVal);
    BOOL PASCAL XlsxDrawingPutFontNameW(DWORD dwID, LPCWSTR newVal);
    BOOL PASCAL XlsxDrawingPutFontPoint(DWORD dwID, double newVal);
    BOOL PASCAL XlsxDrawingPutFontColor(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutFontStyle(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutFontULine(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutTextAnchor(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutTextVertical(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutTextAlign(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingAddImage(DWORD dwID, LPCSTR pszFileName, double sxoff, double syoff, 
		double exoff, double eyoff, double width, double height);
    BOOL PASCAL XlsxDrawingAddImageW(DWORD dwID, LPCWSTR pszFileName, double sxoff, double syoff, 
		double exoff, double eyoff, double width, double height);
    BOOL PASCAL XlsxDrawingAddLine(DWORD dwID, int nLineType, double sxoff, double syoff, 
		double exoff, double eyoff, double width, double height);
    BOOL PASCAL XlsxDrawingAddShape(DWORD dwID, int nShapeType, double sxoff, double syoff, 
		double exoff, double eyoff, double width, double height);
    BOOL PASCAL XlsxDrawingAddTextBox(DWORD dwID, int nShapeType, LPCSTR pszText, 
		double sxoff, double syoff, double exoff, double eyoff, double width, double height);
    BOOL PASCAL XlsxDrawingAddTextBoxW(DWORD dwID, int nShapeType, LPCWSTR pszText, 
		double sxoff, double syoff, double exoff, double eyoff, double width, double height);
	// Xlsx.イベント
    BOOL PASCAL XlsxSetOnErrorProc(DWORD dwID, XLSX_ONERRORPROC proc, LPARAM lParam);

#ifdef  __cplusplus
}
#endif //__cplusplus

//////////////////////////////////////////////////////////////////////////////

#endif	// __EXCELCRTOCX_H_
#endif	// __EXCELOCX_IDL_
#endif	// __CREATOROCX_RC_

//////////////////////////////////////////////////////////////////////////////

#endif	// __XLSXCRT_H
