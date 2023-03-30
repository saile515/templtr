## Options

* target &lt;string&gt; specifies the target of the http request.
* responseSelector &lt;string, optional&gt; a selector query of which to replace the innerHTML with the message. Uses document.querySelectorAll internally.

## Example

```
{
	"$exampleEvent": {
		"type": "wsListen",
		"target": "ws://example.com/",
		"responseSelector": "#result"
	}
}
```