/* SPDX-License-Identifier: LGPL-3.0-or-later */

/*
 * Copyright (C) 2008 Banco do Brasil S.A.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

 #include "private.h"
 #include <functional>
 #include <lib3270/ipc/host.h>

/*--[ Implement ]------------------------------------------------------------------------------------*/

 static DWORD action(std::function<void(TN3270::Host &)> worker) noexcept {

 	try {

		worker(getSession());

		return HLLAPI_STATUS_SUCCESS;

	} catch(const std::system_error &e) {

		return hllapi_translate_error(e);

	} catch(const std::exception &e) {

		hllapi_lasterror = e.what();

	} catch(...) {

		hllapi_lasterror = _( "Unexpected error" );

	}

	return HLLAPI_STATUS_SYSTEM_ERROR;

 }

 static DWORD action(const TN3270::KeyboardAction id) noexcept {

	return action([id](TN3270::Host &host) {
		host.push(id);
	});

 }

 HLLAPI_API_CALL hllapi_enter(void) {
	return action(TN3270::ENTER);
 }

 HLLAPI_API_CALL hllapi_erase(void) {
 	return action(TN3270::ERASE);
 }

 HLLAPI_API_CALL hllapi_erase_eof(void) {
 	return action(TN3270::ERASE_EOF);
 }

 HLLAPI_API_CALL hllapi_erase_eol(void) {
 	return action(TN3270::ERASE_EOL);
 }

 HLLAPI_API_CALL hllapi_erase_input(void) {
 	return action(TN3270::ERASE_INPUT);
 }

 HLLAPI_API_CALL hllapi_kybdreset(void) {
 	return action(TN3270::KYBD_RESET);
 }

 HLLAPI_API_CALL hllapi_action(LPSTR action_name) {

 	return action([action_name](TN3270::Host &host) {
		host.action(action_name);
	});

 }

 HLLAPI_API_CALL hllapi_print(void) {

 	return action([](TN3270::Host &host) {
		host.print();
	});

 }
