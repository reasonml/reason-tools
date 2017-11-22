let stylesheet = {|
  .root-container {
    position: fixed;
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;
    z-index: 65535;
  }

  .mask-container {
    position: absolute;
    box-sizing: border-box;
    padding: 5vw;
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;
    display: flex;
    flex-direction: column;
    justify-content: center;
    background: rgba(0, 0, 0, 0.5);
    transition: background 4000ms;
  }

  .listing-container,
  .error-message-container {
    display: flex;

    box-shadow: 5px 5px 7px 0 rgba(0, 0, 0, 0.3);

    font-family: 'Helvetica', 'Open Sans','Helvetica Neue',Helvetica,Arial,sans-serif;
    font-size: 14px;
    line-height: 20px;
    background-color: #2b303b;
    color: #c0c5ce;

    font-weight: normal; /* external styles leaking in, why? */

    margin: 0 auto;
    margin-bottom: 2vh;
    max-width: 1200px;
  }

  .error-message-container {
    padding: 1em 2em;
    color: #dd4b39;
  }

  .listing-container > .sidebar {
    background: #242831;
    padding: 1em;
    color: #5F6269;
    border-left: 2px solid transparent;
  }
  .listing-container.re > .sidebar {
    border-color: #dd4b39;
  }
  .listing-container.ml > .sidebar {
    border-color: #c87a27;
  }

  .listing-container > .main {
    width: 100%;
  }

  .listing-container > .main > pre {
    padding: 1em 2em;
    overflow: hidden;
    maxHeight: 45vh;
  }

  .listing-container > .main > footer {
    display: flex;
    align-items: center;
    justify-content: flex-end;
    font-size: .95em;
    padding: .5em;
    passin-tTop: 0;
  }
  .listing-container.re > .main > footer {
    color: #dd4b39;
    fill: #dd4b39;
  }
  .listing-container.ml > .main > footer {
    color: #c87a27;
    fill: #c87a27;
  }

  .listing-container > .main > footer > a {
    color: #51555E;
    margin-right: 1.5em;
  }

  .listing-container > .main > footer > a.open-button {
    fill: #51555E;
    margin-right: 1em;
    height: 1em;
    width: 1em;
    margin-top: 1px; /* looks a bit off-center without */
  }

  .listing-container > .main > footer > a:hover {
    color: #B1B3B7;
    fill: #B1B3B7;
    cursor: pointer;
  }
|};
