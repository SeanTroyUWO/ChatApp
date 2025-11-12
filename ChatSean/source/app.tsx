import React from 'react';
import { Text } from 'ink';

type Props = {
	username?: string;
	password?: string;
};

type User = {
	username: string,
	password: string,
	name: string,
	email: string
};

export default function App({ username, password }: User) {
	// POST request to /login/ with the username and password
	// Check if user is successfully signed in
	let loggedIn: boolean = false;
	// If not, we will show them a message
	if (!loggedIn) {
		return (
			<Text color="red">
				Oops! Wrong username and/or password. Please try again.
			</Text>
		);
	}

	// GET request to /users/ to get all users
	let all_users: User[];

	return (
		<Text>
			Hello, <Text color="green">{username}/{password}</Text>
		</Text>
	);
}
