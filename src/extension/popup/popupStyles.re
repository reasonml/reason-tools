let popup =
  ReactDOMRe.Style.make
    display::"flex"
    flex::"1"
    flexDirection::"row"
    justifyContent::"space-around"
    overflow::"hidden"
    ();

let popupColumn =
  ReactDOMRe.Style.make
    display::"flex" flexDirection::"column" paddingBottom::"1%" position::"relative" ();

let popupContext =
  ReactDOMRe.Style.make
    alignItems::"center"
    display::"flex"
    flex::"1"
    fontFamily::"sans-serif"
    fontSize::"3vh"
    justifyContent::"space-between"
    maxHeight::"25px"
    minHeight::"16px"
    paddingBottom::"0.5vh"
    paddingTop::"0.5vh"
    width::"100%"
    ();

let copyConfirmation =
  ReactDOMRe.Style.make
    position::"absolute"
    zIndex::"10"
    top::"50%"
    left::"50%"
    transform::"translate(-50%,-50%)"
    padding::"4vh 8vh 6vh"
    display::"flex"
    flexDirection::"column"
    justifyContent::"center"
    alignItems::"center"
    borderRadius::"2vh"
    backgroundColor::"rgba(0,0,0,0.8)"
    color::"white"
    fontFamily::"sans-serif"
    /*pointerEvents::"none"*/
    ();

let contextLink =
  ReactDOMRe.Style.make
    textDecoration::"none"
    color::"rgb(219, 76, 63)"
    cursor::"pointer"
    fontSize::"16px"
    lineHeight::"16px"
    paddingLeft::"1vw"
    ();

let contextIcon =
  ReactDOMRe.Style.make
    paddingLeft::"1vw"
    cursor::"pointer" /*fill::"rgb(219, 76, 63)"*/
    height::"16px"
    width::"16px"
    ();

let contextTitle = ReactDOMRe.Style.make flex::"1" ();
