#!/usr/bin/env node
import React from 'react';
import { render } from 'ink';
import meow from 'meow';
import App from './app.js';

const cli = meow(
	`
	Usage
	  $ ChatSean

	Options
		--name  Your name

	Examples
	  $ ChatSean --name=Jane
	  Hello, Jane
`,
	{
		importMeta: import.meta,
		flags: {
			username: {
				type: 'string',
			},
			password: {
				type: 'string',
			}
		},
	},
);

render(
	<App
		username={cli.flags.username}
		password={cli.flags.password}
	/>
);
