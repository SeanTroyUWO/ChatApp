import React, { useState, useEffect } from 'react';
import { Text } from 'ink';
import 'dotenv/config';
import SelectInput from 'ink-select-input';

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

export default function App({ username, password }: Props) {
	// UseState for users and logged in
	const [users, setUsers] = useState<User[]>([]);
	const [loggedIn, setLoggedIn] = useState<boolean>(true);
	const [error, setError] = useState<string | null>(null);

	const BASE_URL = process.env['BACKEND_URL'];

	// POST request to /login/ with the username and password

	// If not, we will show them a message
	if (!loggedIn) {
		return (
			<Text color="red">
				Oops! Wrong username and/or password. Please try again.
			</Text>
		);
	}

	// GET request to /users/ to get all users
	// let all_users: User[];
	useEffect(() => {
		if (loggedIn) {
			fetch(`${BASE_URL}/users`)
				.then(res => res.json())
				.then((data: User[]) => setUsers(data))
				.then(() => setLoggedIn(true))
				.catch(err => setError(err.message));
		}
	}, [loggedIn]);

	if (error) {
		return <Text color="red">Oops! {error}</Text>;
	}

	// Convert users into SelectInput items
	const items = users.map(user => ({
		label: `${user.name} (${user.username})`,
		value: user,
		key: user.username
	}));

	return (
		<>
			<Text>
				Hello, <Text color="green">{username}/{password}</Text>
			</Text>
			<SelectInput
				items={items}
				onSelect={item => {
					const user = item.value as User;
					console.log('Selected user:', user);
				}}
			/>
		</>
	);
}
