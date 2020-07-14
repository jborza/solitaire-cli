# coding: utf-8

from scene import *
import sound
import random

# https://www.youtube.com/watch?v=xpevESx3wkc&list=PL_38rYkBIiiXPxs5FbuGo4zZh-WHlnmZg&index=15

# standing and walking texture
standing = Texture('plf:AlienGreen_front')
walking = [Texture('plf:AlienGreen_walk1'), Texture('plf:AlienGreen_walk2')]
hit_texture = Texture('plf:AlienGreen_duck')

class Coin(SpriteNode):
	def __init__(self, **kwargs):
		SpriteNode.__init__(self, 'plf:Item_CoinGold', **kwargs)

class Meteor(SpriteNode):
	def __init__(self, **kwargs):
		img = random.choice(['spc:MeteorBrownBig1','spc:MeteorBrownBig2'])
		SpriteNode.__init__(self, img, **kwargs)


class Game(Scene):
	
	def setup(self):
		self.background_color = "#3b21b5"
		ground = Node(parent=self)
		x = 0
		while x <= self.size.w +64:
			tile = SpriteNode('plf:Ground_Dirt', position=(x,10))
			ground.add_child(tile)
			x += 64
		# create player sprite
		self.player = SpriteNode('plf:AlienGreen_front')
		# position
		self.player.position = (self.size.w/2, 41)
		# anchor
		self.player.anchor_point = (0.5, 0)
		# attach the player the ground (make it visible)
		ground.add_child(self.player)
		# Add score
		self.label_score = LabelNode('0', ('futura',40), parent=self)
		self.label_score.position = (self.size.w / 2, self.size.h -40)
		self.list_of_items = []
		self.new_game()
		
	def new_game(self):
		for item in self.list_of_items:
			item.remove_from_parent()
		self.score = 0
		self.walk_state = -1
		self.list_of_items = []
		self.game_over = False
		self.label_score.text = '0'
		self.player.position = self.size.w / 2, 41
		self.speed = 1
		self.player.texture = standing
		
	def update(self):
		if self.game_over:
			return
		if random.random() < .05:
			self.spawn_items()
		self.update_player()
		 #look for collisions
		self.collisions_with_items()
		
	def update_player(self):
		g = gravity()
		
		self.player.x_scale = ((g.x > 0) - (g.x < 0))
		
		if abs(g.x) > 0.05:
			speed = g.x * 50
			x = self.player.position.x
			x = max(0, min(self.size.w, x + speed))
			self.player.position = x, 41
			step = int(self.player.position.x / 40) % 2
			if step != self.walk_state:
				self.player.texture = walking[step]
				sound.play_effect('rpg:Footstep00', 0.05, 1.0 + .5 * g.x)
				self.walk_state = step
				
		else:
			self.player.texture = standing
			self.walk_state = -1
		
	def spawn_items(self):
		if random.random() < 0.3:
			meteor = Meteor(parent=self)
			meteor.position = random.uniform(20, self.size.w), self.size.h
			duration = random.uniform(2,4)
			meteor.run_action(
				Action.sequence(
					Action.move_to(0,-1000, duration),
					Action.remove()
					)
				)
			# list of coins falling
			self.list_of_items.append(meteor)
		
		else:
			coin = Coin(parent=self)
			coin.position = random.uniform(20, self.size.w), self.size.h
			duration = random.uniform(2,4)
			coin.run_action(
				Action.sequence(
					Action.move_by(0,-1000, duration),
					Action.remove()
					)
				)
			# list of coins falling
			self.list_of_items.append(coin)
			
		
	# Coins collision intersection
	def collisions_with_items(self):
		p_box = Rect(self.player.position.x-20, 32, 40, 65)
		for item in self.list_of_items:
			if item.frame.intersects(p_box):
				if isinstance(item, Coin):
					sound.play_effect('arcade:Coin_2')
					item.remove_from_parent()
					self.list_of_items.remove(item)
					self.score += 10
					self.label_score.text = str(self.score)
				else:
					self.player_hit()
					
	
	def player_hit(self):
		self.game_over = True
		sound.play_effect('arcade:Explosion_6')
		self.player.texture = hit_texture
		self.player.run_action(Action.move_by(0,-150))
		self.run_action(Action.sequence(Action.wait(2*self.speed), Action.call(self.new_game)))
		
		
	
	
			
				
	
	def touch_began(self, touch):
		# load the sprite
		laser = SpriteNode('plf:LaserPurpleDot',
			position=self.player.position,
			z_position= -1,
			parent=self)
			
		 # moving the laser
		laser.run_action(Action.sequence(
			Action.move_by(0,1000),
			Action.remove()))
		
		# sound to the laser
		
		sound.play_effect('arcade:Laser_1')
		
		

		
						
run(Game(), PORTRAIT)
