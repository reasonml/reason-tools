type styleT;

external makeStyle :
 azimuth::string? =>
 background::string? =>
 backgroundAttachment::string? =>
 backgroundColor::string? =>
 backgroundImage::string? =>
 backgroundPosition::string? =>
 backgroundRepeat::string? =>
 border::string? =>
 borderCollapse::string? =>
 borderColor::string? =>
 borderSpacing::string? =>
 borderStyle::string? =>
 borderTop::string? =>
 borderRight::string? =>
 borderBottom::string? =>
 borderLeft::string? =>
 borderTopColor::string? =>
 borderRightColor::string? =>
 borderBottomColor::string? =>
 borderLeftColor::string? =>
 borderTopStyle::string? =>
 borderRightStyle::string? =>
 borderBottomStyle::string? =>
 borderLeftStyle::string? =>
 borderTopWidth::string? =>
 borderRightWidth::string? =>
 borderBottomWidth::string? =>
 borderLeftWidth::string? =>
 borderWidth::string? =>
 bottom::string? =>
 captionSide::string? =>
 clear::string? =>
 clip::string? =>
 color::string? =>
 content::string? =>
 counterIncrement::string? =>
 counterReset::string? =>
 cue::string? =>
 cueAfter::string? =>
 cueBefore::string? =>
 cursor::string? =>
 direction::string? =>
 display::string? =>
 elevation::string? =>
 emptyCells::string? =>
 cssFloat::string? =>
 font::string? =>
 fontFamily::string? =>
 fontSize::string? =>
 fontSizeAdjust::string? =>
 fontStretch::string? =>
 fontStyle::string? =>
 fontVariant::string? =>
 fontWeight::string? =>
 height::string? =>
 left::string? =>
 letterSpacing::string? =>
 lineHeight::string? =>
 listStyle::string? =>
 listStyleImage::string? =>
 listStylePosition::string? =>
 listStyleType::string? =>
 margin::string? =>
 marginTop::string? =>
 marginRight::string? =>
 marginBottom::string? =>
 marginLeft::string? =>
 markerOffset::string? =>
 marks::string? =>
 maxHeight::string? =>
 maxWidth::string? =>
 minHeight::string? =>
 minWidth::string? =>
 orphans::string? =>
 outline::string? =>
 outlineColor::string? =>
 outlineStyle::string? =>
 outlineWidth::string? =>
 overflow::string? =>
 padding::string? =>
 paddingTop::string? =>
 paddingRight::string? =>
 paddingBottom::string? =>
 paddingLeft::string? =>
 page::string? =>
 pageBreakAfter::string? =>
 pageBreakBefore::string? =>
 pageBreakInside::string? =>
 pause::string? =>
 pauseAfter::string? =>
 pauseBefore::string? =>
 pitch::string? =>
 pitchRange::string? =>
 playDuring::string? =>
 position::string? =>
 quotes::string? =>
 richness::string? =>
 right::string? =>
 size::string? =>
 speak::string? =>
 speakHeader::string? =>
 speakNumeral::string? =>
 speakPunctuation::string? =>
 speechRate::string? =>
 stress::string? =>
 tableLayout::string? =>
 textAlign::string? =>
 textDecoration::string? =>
 textIndent::string? =>
 textShadow::string? =>
 textTransform::string? =>
 top::string? =>
 unicodeBidi::string? =>
 verticalAlign::string? =>
 visibility::string? =>
 voiceFamily::string? =>
 volume::string? =>
 whiteSpace::string? =>
 widows::string? =>
 width::string? =>
 wordSpacing::string? =>
 zIndex::string? =>

 /* CSS3 Props */
 transform::string? =>
 transition::string? =>
 opacity::string? =>

 unit =>
 styleT = "" [@@bs.obj];

external styleToObj : styleT => Js.t {..} = "%identity";

module Transition = {
  include ReactRe.Component.Stateful;
  let name = "Transition";

  type props = {
    before: styleT,
    after: styleT,
    children: list ReactRe.reactElement
  };
  type state = {
    style: styleT
  };

  let getInitialState props => {
    style: props.before
  };

  let componentDidMount {updater} => {
    Core.Util.setTimeout (updater (fun {props} _ => Some { style: props.after })) 0;
    None
  };

  let render {props, state} =>
    <div style=(styleToObj state.style)>
      (ReactRe.arrayToElement (Array.of_list props.children))
    </div>
};

include ReactRe.CreateComponent Transition;
let createElement ::before ::after ::children =>
  wrapProps { before, after, children } ::children;
