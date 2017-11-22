let loaded = ref(false);

Protocol.Storage.queryDisabled(
  (disabled) =>
    if (! disabled && Detect.shouldConvert()) {
      Protocol.LoadScripts.send()
    }
);

Protocol.NotifyLoaded.listen(() => loaded := true);

Protocol.QueryLoaded.listen(() => loaded^);
