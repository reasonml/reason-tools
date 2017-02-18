let loaded = ref false;

if (Detect.mightBeOcamlDoc ()) {
  Protocol.LoadScripts.send ();
};

Protocol.NotifyLoaded.listen (fun () => loaded := true);
Protocol.QueryLoaded.listen (fun () => !loaded);
