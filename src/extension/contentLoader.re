let loaded = ref false;

Protocol.Storage.queryDisabled (fun disabled => {
  if (not disabled && Detect.shouldConvert ()) {
    Protocol.LoadScripts.send ();
  };
});

Protocol.NotifyLoaded.listen (fun () => loaded := true);
Protocol.QueryLoaded.listen (fun () => !loaded);
