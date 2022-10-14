'''Module for testing the quality_assignment module.
I'm sure there's some way to refactor the code so
    client.get('/logout')
    admin_user = client.post('/login', data={
        'email': 'admin@msn.com',
        'user_pass': 'seed'
    })
isn't called in almost every method.
Also need to do some refactoring.
I'll check these/do these tomorrow if I can.

See failed_tests.py for tests I wanted to run but couldn't get to work.'''

import sqlite3
import pytest
from quality_assignment import app, get_db_connection
from initdb import init_db

#Will use testing database so the original database isn't touched.

TEST_DATABASE = 'test_database.db'

@pytest.fixture
def client():
    init_db(TEST_DATABASE)

    #Makes it so the database being accessed by these tests is not the production database
    app.config['DB_LOCATION'] = TEST_DATABASE
    app.config['TESTING'] = True

    yield app.test_client()

@pytest.fixture(autouse=True)
def db_conn():
    '''Fixture that returns a connection to the testing database.'''

    conn = sqlite3.connect(TEST_DATABASE)
    conn.row_factory = sqlite3.Row
    return conn

@pytest.fixture(autouse=True)
def admin_user(client):
    '''Returns the user with username = admin from the database.'''

    client.get('/logout')
    admin_user = client.post('/login', data={
        'email': 'admin@msn.com',
        'user_pass': 'seed'
    })
    return admin_user

@pytest.fixture(autouse=True)
def regular_user(client):
    '''Returns the user with username = bob from the database.'''

    client.get('/logout')
    regular_user = client.post('/login', data={
        'email': 'bob@gmail.com',
        'user_pass': 'bees'
    })
    return regular_user

def test_user_exists(admin_user):
    '''Checks that a given user exists in the database.'''

    assert admin_user

@pytest.mark.xfail
def test_user_does_not_exist(client):
    '''Checks that a given user does not exist in the database. Could not get below format to work:

    client.get('/logout')
    missing_user = client.post('/login', data={
        'email': 'anya@gmail.com',
        'user_pass': 'bees'
    })'''

    conn = get_db_connection()
    missing_user = conn.execute('SELECT * FROM users WHERE user = ?', ('anya',)).fetchone()
    assert missing_user

def test_get_valid_post(client):
    '''Checks logged in user can fetch a valid post'''

    client.get('/logout')
    client.post('/login', data={
        'email': 'admin@msn.com',
        'user_pass': 'seed'
    })
    resp = client.get('/1/edit')
    assert resp.status_code == 200

def test_get_valid_post(client):
    '''Checks user cannot fetch invalid post'''

    client.get('/logout')
    client.post('/login', data={
        'email': 'admin@msn.com',
        'user_pass': 'seed'
    })
    resp = client.get('/-1/edit')
    assert resp.status_code != 200

def test_home_page(client):
    '''Checks that the home page displays.'''

    resp = client.get('/home')
    assert resp.status_code == 200

def test_home_page_alt(client):
    '''Checks that the home page redirects to /home from /.'''

    resp = client.get('/', follow_redirects = True)
    assert len(resp.history) == 1
    assert resp.request.path == '/home'

def test_user_page(client):
    '''Checks that all of posts are displayed with /admin, and none of bob's.'''

    resp = client.get('/admin')
    assert b'Author: admin' in resp.data
    assert b'Author: bob' not in resp.data

def test_invalid_user_page(client):
    '''Checks that 'No posts found' is displayed with invalid /path'''

    resp = client.get('/q')
    assert b'No posts found' in resp.data

def test_about_page(client):
    '''Checks that the about page displays.'''

    resp = client.get('/about')
    assert b'Click on one of the users posted below to see all of their posts!' in resp.data

def test_about_page_displays_emails_to_admin(client, admin_user):
    '''Checks to see if the admin can see emails of the registered users.'''

    client.get('/logout')
    client.post('/login', data={
        'email': 'admin@msn.com',
        'user_pass': 'seed'
    })
    resp = client.get('/about')
    assert b'admin@msn.com' in resp.data

def test_about_page_hidess_emails_from_regular_user(client, admin_user):
    '''Checks to see if the admin can see emails of the registered users.'''

    client.get('/logout')
    client.post('/login', data={
        'email': 'bob@gmail.com',
        'user_pass': 'bees'
    })
    resp = client.get('/about')
    assert b'admin@msn.com' not in resp.data

def test_create_page(client):
    '''Checks that visiting /create displays a form with title + content input fields
    to a logged in user.'''

    client.get('/logout')
    client.post('/login', data={
        'email': 'admin@msn.com',
        'user_pass': 'seed'
    })
    resp = client.get('/create')
    assert b'<label for="title">Title</label>' in resp.data
    
def test_create_page_when_logged_out(client):
    '''Checks that visiting /create flashes 'You must be logged in to create posts',
    and only a button to login.'''

    client.get('/logout')
    resp = client.get('/create')
    assert b'<label for="title">Title</label>' not in resp.data
    assert b'Must be logged in to create posts.' in resp.data
    assert b'<a href ="/login"><button>Login</button></a>' in resp.data

def test_successful_post_creation(client):
    '''Checks to see if a user can create a post successfully.'''

    client.get('/logout')
    client.post('/login', data={
        'email': 'admin@msn.com',
        'user_pass': 'seed'
    })
    resp = client.post('/create', follow_redirects = True, data = {
            'title': 'Testing',
            'content': 'Some test content.'
    })
    assert len(resp.history) == 1
    assert resp.request.path == '/home'

def test_post_creation_with_empty_title_field(client):
    '''Checks to see create flashes an error when title field is empty.'''

    client.get('/logout')
    client.post('/login', data={
        'email': 'admin@msn.com',
        'user_pass': 'seed'
    })
    resp = client.post('/create', follow_redirects = True, data = {
            'title': '',
            'content': 'Some test content.'
    })

    assert b'Title is required!' in resp.data
    assert resp.request.path == '/create'

def test_post_creation_with_empty_content_field(client):
    '''Checks to see create flashes an error when content field is empty.'''
    
    client.get('/logout')
    client.post('/login', data={
        'email': 'admin@msn.com',
        'user_pass': 'seed'
    })
    resp = client.post('/create', follow_redirects = True, data = {
            'title': 'Test',
            'content': ''
    })

    assert b'Content is required!' in resp.data
    assert resp.request.path == '/create'

def test_login_page(client):
    '''Checks that the login page displays to a logged out user.'''

    client.get('/logout')
    resp = client.get('/login')
    assert b'Already logged in. Please log out first.' not in resp.data
    assert resp.request.path == '/login'

def test_login_page_while_logged_in(client):
    '''Checks that the logged in user cannot view the login page.'''

    client.get('/logout')
    client.post('/login', follow_redirects=True, data={
        'email': 'admin@msn.com',
        'user_pass': 'seed'
    })
    resp = client.get('login', follow_redirects = True)
    assert b'Already logged in. Please log out first.' in resp.data
    assert len(resp.history) == 1
    assert resp.request.path == '/home'

def test_admin_login(client):
    '''Checks that the admin user can login and app redirects to about page.'''

    client.get('/logout')
    resp = client.post('/login', follow_redirects=True, data={
        'email': 'admin@msn.com',
        'user_pass': 'seed'
    })
    assert len(resp.history) == 1
    assert resp.request.path == '/about'

def test_user_login(client):
    '''Checks that the bob user can login and app redirects to home page.'''

    client.get('/logout')
    resp = client.post('/login', follow_redirects=True, data={
        'email': 'bob@gmail.com',
        'user_pass': 'bees'
    })
    assert len(resp.history) == 1
    assert resp.request.path == '/home'

def test_incorrect_email_login_1(client):
    '''Checks that login page flashes 'user not found' error if email field left blank.'''

    client.get('/logout')
    resp = client.post('/login', follow_redirects=True, data={
        'email': '',
        'user_pass': 'bees'
    })
    assert b'User does not exist. Please register.' in resp.data
    assert resp.request.path == '/login'

def test_incorrect_email_login_2(client):
    '''Checks that login page flashes 'user not found' error if email is incorrect.'''

    client.get('/logout')
    resp = client.post('/login', follow_redirects=True, data={
        'email': 'admin@gmail.com',
        'user_pass': 'bees'
    })
    assert b'User does not exist. Please register.' in resp.data
    assert resp.request.path == '/login'

def test_incorrect_password_login_1(client):
    '''Checks that login page flashes 'incorrect password' error if password field left blank.'''

    client.get('/logout')
    resp = client.post('/login', follow_redirects=True, data={
        'email': 'admin@msn.com',
        'user_pass': ''
    })
    assert b'Incorrect password.' in resp.data
    assert resp.request.path == '/login'

def test_incorrect_password_login_2(client):
    '''Checks that login page flashes 'incorrect password' error if password is incorrect.'''

    client.get('/logout')
    resp = client.post('/login', follow_redirects=True, data={
        'email': 'admin@msn.com',
        'user_pass': 'bees'
    })
    assert b'Incorrect password.' in resp.data
    assert resp.request.path == '/login'

def test_logout_action(client):
    '''Checks that the logout link redirects to home page.'''

    resp = client.get('/logout', follow_redirects = True)
    assert len(resp.history) == 1
    assert resp.request.path == '/home'

def test_guest_logout_action(client):
    '''Checks that an error message flashes if trying to log out while not logged in.'''
    client.get('/logout')
    resp = client.get('/logout', follow_redirects = True)
    assert b'You are not logged in!' in resp.data
    assert len(resp.history) == 1
    assert resp.request.path == '/home'

def test_register_page(client):
    '''Checks that the register page displays.'''

    resp = client.get('/register')
    assert resp.status_code == 200

def test_successful_register(client):
    '''Checks that a user can successfully register.'''

    client.get('/logout')
    resp = client.post('/register', follow_redirects=True, data={
        'username': 'anya',
        'email': 'anya@gmail.com',
        'user_pass': 'bless'
    })
    
    assert len(resp.history) == 1
    assert b'Successfully registered!' in resp.data
    assert resp.request.path == '/login'

    resp = client.post('/login', follow_redirects=True, data={
        'email': 'anya@gmail.com',
        'user_pass': 'bless'
    })

    assert b'User does not exist. Please register.' not in resp.data
    assert len(resp.history) == 1
    assert resp.request.path == '/home'

def test_register_empty_username(client):
    '''Checks that a user cannot leave the username field blank.'''

    resp = client.post('/register', data={
        'username': '',
        'email': 'lia@gmail.com',
        'user_pass': 'bsa'
    })
    assert resp.request.path == '/register'
    assert b'Username field cannot be blank.' in resp.data

def test_register_empty_email(client):
    '''Checks that a user cannot leave the email field blank.'''

    resp = client.post('/register', data={
        'username': 'lia',
        'email': '',
        'user_pass': 'bsa'
    })
    assert resp.request.path == '/register'
    assert b'Email field cannot be blank.' in resp.data

def test_register_empty_password(client):
    '''Checks that a user cannot leave the password field blank.'''
    
    resp = client.post('/register', data={
        'username': 'lia',
        'email': 'lia@gmail.com',
        'user_pass': ''
    })
    assert resp.request.path == '/register'
    assert b'Password field cannot be blank.' in resp.data

def test_register_incorrect_email_format_1(client):
    '''Checks that a user cannot enter an incorrectly formatted email.
    Incorrectly Formatted email: no @'''

    resp = client.post('/register', data={
        'username': 'lia',
        'email': 'abc',
        'user_pass': 'bsa'
    })
    assert resp.request.path == '/register'
    assert b'Incorrect email format.' in resp.data

def test_register_incorrect_email_format_2(client):
    '''Checks that a user cannot enter an incorrectly formatted email.
    Incorrectly Formatted email: 2 @s'''

    resp = client.post('/register', data={
        'username': 'lia',
        'email': 'abc@@',
        'user_pass': 'bsa'
    })
    assert resp.request.path == '/register'
    assert b'Incorrect email format.' in resp.data

def test_register_incorrect_email_format_3(client):
    '''Checks that a user cannot enter an incorrectly formatted email.
    Incorrectly Formatted email: 2 .s after singular @'''

    resp = client.post('/register', data={
        'username': 'lia',
        'email': 'abc@gmail..com',
        'user_pass': 'bsa'
    })
    assert resp.request.path == '/register'
    assert b'Incorrect email format.' in resp.data

def test_register_incorrect_email_format_4(client):
    '''Checks that a user cannot enter an incorrectly formatted email.
    Incorrectly Formatted email: 0 .s after singular @'''

    resp = client.post('/register', data={
        'username': 'lia',
        'email': 'abc@gmailcom',
        'user_pass': 'bsa'
    })
    assert resp.request.path == '/register'
    assert b'Incorrect email format.' in resp.data

def test_register_duplicate_username(client):
    '''Checks that a user cannot register with a duplicate username.'''

    resp = client.post('/register', data={
        'username': 'admin',
        'email': 'a@gmail.com',
        'user_pass': 'bsa'
    })
    assert resp.request.path == '/register'
    assert b'Username admin already registered.' in resp.data

def test_register_duplicate_username(client):
    '''Checks that a user cannot register with a duplicate email.'''

    resp = client.post('/register', data={
        'username': 'lia',
        'email': 'admin@msn.com',
        'user_pass': 'bsa'
    })
    assert resp.request.path == '/register'
    assert b'Email admin@msn.com already registered.' in resp.data