#!/usr/bin/env python

# pylint: disable=unused-argument
# This program is dedicated to the public domain under the CC0 license.

"""
Simple Bot to translate Telegram messages.

First, a few handler functions are defined. Then, those functions are passed to
the Application and registered at their respective places.

Then, the bot is started and runs until we press Ctrl-C on the command line.
"""
import argostranslate.package
import argostranslate.translate

import logging
import os
import sys


from telegram import ReplyKeyboardMarkup, ReplyKeyboardRemove, ForceReply, Update
from telegram.ext import (
        Application, 
        CommandHandler, 
        ContextTypes, 
        MessageHandler, 
        filters, 
        ConversationHandler
    )


from_code = "en"
to_code = "uk"


# Enable logging

logging.basicConfig(
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s", level=logging.INFO
)

# set higher logging level for httpx to avoid all GET and POST requests being logged
logging.getLogger("httpx").setLevel(logging.WARNING)
logger = logging.getLogger(__name__)

# Define a few command handlers. These usually take the two arguments update and
# context.
async def start(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    """Send a message when the command /start is issued."""
    user = update.effective_user
    await update.message.reply_html(
        rf"Hi {user.mention_html()}!",
        reply_markup=ForceReply(selective=True),
    )


async def translate(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    """Respond to the user message."""

    if update.message is None:
        return
    if update.message.text is None:
        await update.message.reply_text("Я вмію лише перекладати текстові повідомлення.")
        return

    logger.info("Received: %s", update.message.text)
    # Translate
    translatedText = argostranslate.translate.translate(update.message.text, from_code, to_code)
    logger.info("Translated: %s", translatedText)
    await update.message.reply_text(translatedText)


def main() -> None:
    """Start the bot."""
    # Download and install Argos Translate package
    logger.info("Updating Argos Translate package index...")
    argostranslate.package.update_package_index()
    available_packages = argostranslate.package.get_available_packages()
    # package_to_install = next(
    #     filter(
    #         lambda x: x.from_code == from_code and x.to_code == to_code, available_packages
    #     )
    # )
    logger.info("Installing packages...")
    for package in available_packages:
        logger.info("Package: %s", package)
        argostranslate.package.install_from_path(package.download())
    logger.info("Installed packages")

    # Create the Application and pass it your bot's token.
    token = os.environ.get("BOT_TOKEN")
    if token is None:
        logger.error("Please specify the Telegram bot token using the BOT_TOKEN environment variable.")
        sys.exit(78)

    logger.info("Token: %s", token)
    application = Application.builder().token(token).build()

    # on different commands - answer in Telegram
    application.add_handler(CommandHandler("start", start))

    # on non command i.e message - translate the message on Telegram
    application.add_handler(MessageHandler(filters.TEXT & ~filters.COMMAND, translate))

    # Run the bot until the user presses Ctrl-C
    application.run_polling(allowed_updates=Update.MESSAGE)


if __name__ == "__main__":
    main()
