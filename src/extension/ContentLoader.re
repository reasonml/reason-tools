open Core;

let loaded = ref false;

let load () =>
  Message.send "background:load-content-scripts" ();

if (Detect.mightBeOcamlDoc ()) {
  load ();
};

Message.receive "content:notify-loaded" (fun _ _ _ => loaded := true);
Message.receive "content:query-loaded" (fun _ _ respond => respond !loaded);
